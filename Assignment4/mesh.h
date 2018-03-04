#ifndef MESH_H
#define MESH_H

struct vertex
{
	vec3 pos;
	vec3 norm;
	vec2 tex;
};

struct Material
{
	vec4	ambient;
	vec4	diffuse;
	vec4	specular;
	float	shininess;
};

struct Mesh
{
	GLuint				vertexBuffer;
	GLuint				indexBuffer;
	std::vector<vertex>	vertexList;
	std::vector<uint>	indexList;
};

typedef struct TriangleIndices
{
    int v1;
    int v2;
    int v3;

    TriangleIndices(int v1, int v2, int v3)
    {
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
    }
}TriangleIndices;

typedef struct DictionaryCell
{
	long long key;
	int index;

	DictionaryCell(long long key, int index)
	{
		this->key = key;
		this->index = index;
	}	
}DictionaryCell;


Mesh* loadMesh( const char* vertexFilePath, const char* indexFilePath );

#endif //MESH_H
