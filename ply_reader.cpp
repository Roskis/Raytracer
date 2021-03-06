// This code is "Public Domain", no rights reserved.

#include "ply_reader.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <string.h>
#include <assert.h>

using namespace std;

std::unique_ptr<PLY_Model> Read_PLY_Model(const char *filename)
{
	FILE *file = fopen(filename, "r");

	// Error checks omitted for simplicity.
	assert(file);

	// Parse header
	char header_field[1024] = "\0";
	int  vertex_count = 0;
	int  face_count = 0;

	while (strcmp(header_field, "end_header"))
	{
		if (fscanf(file, "%s", header_field) != 1) {
			std::cout << "read failure" << std::endl;
		}

		if (!strcmp(header_field, "vertex")) {
			if (fscanf(file, "%d", &vertex_count) != 1) {
				std::cout << "read failure" << std::endl;
			}
		}

		if (!strcmp(header_field, "face")) {
			if (fscanf(file, "%d", &face_count) != 1) {
				std::cout << "read failure" << std::endl;
			}
		}
	}

	// Construct the target buffers
	std::unique_ptr<PLY_Model> res = std::make_unique<PLY_Model>();
	res->m_vertex_data.reserve(vertex_count);
	res->m_face_data.reserve(face_count);

	// Read vertex data
	for (int i = 0; i < vertex_count * 3; ++i)
	{
		float val = 0;
		if (fscanf(file, "%f", &val) != 1) {
			std::cout << "read failure" << std::endl;
		}
		res->m_vertex_data.push_back(val);
	}

	// Read face (triangles) data
	for (int i = 0; i < face_count * 4; ++i)
	{
		int val = 0;
		if (fscanf(file, "%d", &val) != 1) {
			std::cout << "read failure" << std::endl;
		}

		if (i % 4 == 0)
		{
			// Beginning of the face data row, assert that we have triangles
			assert(val == 3);
		}
		else
		{
			res->m_face_data.push_back(val);
		}
	}

	fclose(file);

	return res;
}
