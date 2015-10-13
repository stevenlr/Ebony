package main

import(
	"fmt"
	"os"
	"bufio"
	"strings"
	"strconv"
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
		fmt.Errorf("Invalid float")
		os.Exit(1)
	}

	value = float32(v)
	return
}

func parseInt(str string) (value int32) {
	v, err := strconv.ParseInt(str, 10, 32)

	if err != nil {
		fmt.Errorf("Invalid int")
		os.Exit(1)
	}

	value = int32(v)
	return
}

func (v Vector3f) read(tokens []string) {
	if len(tokens) < 3 {
		fmt.Errorf("Invalid 3-vector read")
		os.Exit(1)
	}

	v.x = parseFloat(tokens[0])
	v.y = parseFloat(tokens[1])
	v.z = parseFloat(tokens[2])
}

func (v Vector2f) read(tokens []string) {
	if len(tokens) < 2 {
		fmt.Errorf("Invalid 2-vector read")
		os.Exit(1)
	}

	v.x = parseFloat(tokens[0])
	v.y = parseFloat(tokens[1])
}

func (f Face) read(tokens []string) {
	if len(tokens) < 3 {
		fmt.Errorf("Invalid triangle face")
		os.Exit(1)
	}

	for i := 0; i < 3; i++ {
		f.vertices[i].read(tokens[i])
	}
}

func (v Vertex) read(str string) {
	tokens := strings.Split(str, "/")

	if len(tokens) < 3 {
		fmt.Errorf("Invalid vertex")
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
	// outputFilename := os.Args[2]

	inputFile, err := os.OpenFile(inputFilename, os.O_RDONLY, 0666)

	if err != nil {
		fmt.Fprintf(os.Stderr, "Couldn't open file %s", inputFilename)
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
}