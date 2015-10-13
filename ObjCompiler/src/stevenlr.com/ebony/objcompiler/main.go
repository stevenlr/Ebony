package main

import (
	"bufio"
	"bytes"
	"compress/zlib"
	"encoding/binary"
	"fmt"
	"math"
	"os"
	"strconv"
	"strings"
)

type Vector2f struct {
	x, y float32
}

type Vector3f struct {
	x, y, z float32
}

type Vertex struct {
	pos, uv, normal int32
}

type Face struct {
	vertices [3]Vertex
}

func minusv3f(a, b Vector3f) (v Vector3f) {
	v.x = a.x - b.x
	v.y = a.y - b.y
	v.z = a.z - b.z
	return
}

func minusv2f(a, b Vector2f) (v Vector2f) {
	v.x = a.x - b.x
	v.y = a.y - b.y
	return
}

func (v *Vector3f) normalize() {
	length := float32(math.Sqrt(float64(v.x*v.x + v.y*v.y + v.z*v.z)))
	v.x /= length
	v.y /= length
	v.z /= length
}

func removeComments(line string) string {
	index := strings.IndexByte(line, '#')

	if index != -1 {
		line = line[:index]
	}

	return strings.TrimSpace(line)
}

func tokenizeLine(line string) []string {
	line = strings.Replace(line, "\r", " ", -1)
	line = strings.Replace(line, "\n", " ", -1)
	line = strings.Replace(line, "\t", " ", -1)

	return strings.Split(line, " ")
}

func parseFloat(str string) (value float32) {
	v, err := strconv.ParseFloat(str, 32)

	if err != nil {
		fmt.Fprintf(os.Stderr, "Invalid float")
		os.Exit(1)
	}

	value = float32(v)
	return
}

func parseInt(str string) (value int32) {
	v, err := strconv.ParseInt(str, 10, 32)

	if err != nil {
		fmt.Fprintf(os.Stderr, "Invalid int")
		os.Exit(1)
	}

	value = int32(v)
	return
}

func (v *Vector3f) read(tokens []string) {
	if len(tokens) < 3 {
		fmt.Fprintf(os.Stderr, "Invalid 3-vector read")
		os.Exit(1)
	}

	v.x = parseFloat(tokens[0])
	v.y = parseFloat(tokens[1])
	v.z = parseFloat(tokens[2])
}

func (v *Vector2f) read(tokens []string) {
	if len(tokens) < 2 {
		fmt.Fprintf(os.Stderr, "Invalid 2-vector read")
		os.Exit(1)
	}

	v.x = parseFloat(tokens[0])
	v.y = parseFloat(tokens[1])
}

func (f *Face) read(tokens []string) {
	if len(tokens) < 3 {
		fmt.Fprintf(os.Stderr, "Invalid triangle face")
		os.Exit(1)
	}

	for i := 0; i < 3; i++ {
		f.vertices[i].read(tokens[i])
	}
}

func (v *Vertex) read(str string) {
	tokens := strings.Split(str, "/")

	if len(tokens) < 3 {
		fmt.Fprintf(os.Stderr, "Invalid vertex")
		os.Exit(1)
	}

	v.pos = parseInt(tokens[0]) - 1
	v.uv = parseInt(tokens[1]) - 1
	v.normal = parseInt(tokens[2]) - 1
}

func countData(reader *bufio.Reader) (nbVertex, nbTextures, nbNormals, nbFaces int) {
	nbVertex, nbTextures, nbNormals, nbFaces = 0, 0, 0, 0

	for {
		line, err := reader.ReadString('\n')

		if err != nil {
			break
		}

		line = removeComments(line)

		if len(line) == 0 {
			continue
		}

		tokens := tokenizeLine(line)

		switch tokens[0] {
		case "v":
			nbVertex++
			break
		case "vt":
			nbTextures++
			break
		case "vn":
			nbNormals++
			break
		case "f":
			nbFaces++
			break
		}
	}

	return
}

func gatherData(reader *bufio.Reader, positions []Vector3f, uvs []Vector2f, normals []Vector3f, faces []Face) {
	nbVertex, nbTextures, nbNormals, nbFaces := 0, 0, 0, 0

	for {
		line, err := reader.ReadString('\n')

		if err != nil {
			break
		}

		line = removeComments(line)

		if len(line) == 0 {
			continue
		}

		tokens := tokenizeLine(line)

		switch tokens[0] {
		case "v":
			positions[nbVertex].read(tokens[1:])
			nbVertex++
			break
		case "vt":
			uvs[nbTextures].read(tokens[1:])
			nbTextures++
			break
		case "vn":
			normals[nbNormals].read(tokens[1:])
			nbNormals++
			break
		case "f":
			faces[nbFaces].read(tokens[1:])
			nbFaces++
			break
		}
	}
}

func main() {
	if len(os.Args) < 3 {
		fmt.Fprintf(os.Stderr, "Usage: %s <input file> <output file>", os.Args[0])
		os.Exit(1)
	}

	inputFilename := os.Args[1]
	outputFilename := os.Args[2]

	inputFile, err := os.OpenFile(inputFilename, os.O_RDONLY, 0666)

	if err != nil {
		fmt.Fprintf(os.Stderr, "Couldn't open file %s", inputFilename)
		os.Exit(1)
	}

	inputFileReader := bufio.NewReader(inputFile)
	nbVertex, nbTextures, nbNormals, nbFaces := countData(inputFileReader)

	positions := make([]Vector3f, nbVertex)
	uvs := make([]Vector2f, nbTextures)
	normals := make([]Vector3f, nbNormals)
	faces := make([]Face, nbFaces)

	inputFile.Seek(0, 0)
	inputFileReader.Reset(inputFile)

	gatherData(inputFileReader, positions, uvs, normals, faces)
	inputFile.Close()

	var buffer bytes.Buffer

	binary.Write(&buffer, binary.LittleEndian, int32(len(faces)))

	for i := 0; i < len(faces); i++ {
		face := faces[i]

		p0 := minusv3f(positions[face.vertices[1].pos], positions[face.vertices[0].pos])
		p1 := minusv3f(positions[face.vertices[2].pos], positions[face.vertices[0].pos])
		u0 := minusv2f(uvs[face.vertices[1].uv], uvs[face.vertices[0].uv])
		u1 := minusv2f(uvs[face.vertices[2].uv], uvs[face.vertices[0].uv])

		detinv := 1 / (u0.x*u1.y - u1.x*u0.y)

		var T Vector3f

		T.x = detinv * (p0.x*u1.y - p1.x*u0.y)
		T.y = detinv * (p0.y*u1.y - p1.y*u0.y)
		T.z = detinv * (p0.z*u1.y - p1.z*u0.y)

		T.normalize()

		for j := 0; j < 3; j++ {
			binary.Write(&buffer, binary.LittleEndian, positions[face.vertices[j].pos])
			binary.Write(&buffer, binary.LittleEndian, normals[face.vertices[j].normal])
			binary.Write(&buffer, binary.LittleEndian, T)
			binary.Write(&buffer, binary.LittleEndian, uvs[face.vertices[j].uv])
		}
	}

	outputFile, err := os.OpenFile(outputFilename, os.O_WRONLY | os.O_CREATE | os.O_TRUNC, 0666)

	if err != nil {
		fmt.Fprint(os.Stderr, "Error when opening file %s for writing", outputFilename)
		os.Exit(1)
	}

	zWriter := zlib.NewWriter(outputFile)
	buffer.WriteTo(zWriter)
	zWriter.Close()
	outputFile.Close()
}
