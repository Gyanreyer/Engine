#include "Model.h"



Model::Model()
{
	vertCount = 0;
	vertArr = 0;
}


Model::~Model()
{
}

bool Model::buffer(string objFile)
{

	vector<vec3> locs;
	vector<vec2> uvs;
	vector<vec3> norms;
	vector<vertInd> vertInds;

	ifstream inFile;
	string line;

	inFile.open(objFile);

	if (inFile.is_open())
	{
		while (getline(inFile, line))
		{
			istringstream ss(line);

			string label;
			float fNum[3];

			ss >> label;

			//Location
			if (label == "v")
			{
				ss >> fNum[0] >> fNum[1] >> fNum[2];

				locs.push_back(vec3(fNum[0], fNum[1], fNum[2]));
			}
			//UVs
			else if (label == "vt")
			{
				ss >> fNum[0] >> fNum[1];

				uvs.push_back(vec2(fNum[0], fNum[1]));
			}
			//Normals
			else if (label == "vn")
			{
				ss >> fNum[0] >> fNum[1] >> fNum[2];

				norms.push_back(vec3(fNum[0], fNum[1], fNum[2]));
			}
			//Indices
			else if (label == "f")
			{
				unsigned int iNum[9];
				char slash;

				ss >> iNum[0] >> slash >> iNum[1] >> slash >> iNum[2]
					>> iNum[3] >> slash >> iNum[4] >> slash >> iNum[5]
					>> iNum[6] >> slash >> iNum[7] >> slash >> iNum[8];

				//Decrement indices
				for (int i = 0; i < 9; i++)
				{
					iNum[i]--;
				}

				vertInds.push_back({ iNum[0], iNum[1], iNum[2] });
				vertInds.push_back({ iNum[3], iNum[4], iNum[5] });
				vertInds.push_back({ iNum[6], iNum[7], iNum[8] });
			}

		}

		inFile.close();
	}
	else
	{
		return false;
	}

	vertCount = vertInds.size();//How many vertices there are

	//Buffer data holds vertices with location, uv, and normal data
	vector<vertex> vertBufData(vertCount);

	//Go through all vertices and get buffer data for each one
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = {

			locs[vertInds[i].locInd],
			uvs[vertInds[i].uvInd],
			norms[vertInds[i].normInd] };
	}

	GLuint vertBuf;

	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertex) * vertCount,
		&vertBufData[0],
		GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);

	//Location
	glVertexAttribPointer(0, //Attribute index
		3, //Number of components (x,y,z)
		GL_FLOAT, //Type of data
		GL_FALSE, //Whether data should be normalized
		sizeof(vertex), //Stride (bytes per vertex)
		0); //Offset to this attribute

	glEnableVertexAttribArray(1);

	//UV
	glVertexAttribPointer(1, //Attrib index
		2, //Number of components (u,v)
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertex),
		(void*)sizeof(vec3)); //Offset by size of vertex since this comes second


	//Normal
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(2, 
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertex),
		(void*)(sizeof(vec2) + sizeof(vec3)));

	glBindVertexArray(0);

	return true;
}

void Model::render()
{
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}
