#include "assets.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "logger.hpp"





///////////////////////
// Mesh loading below
///////////////////////

namespace
{
	struct Index
	{
		GLuint v, t, n;
	};

	struct Face
	{
		Index indices[3];
		std::string mtl;
	};
	struct LoadedMaterial
	{
		GLubyte id;

		std::string name;
		glm::vec3 Ka, Kd, Ks;
		GLfloat Ns, Ni, d;
		GLubyte illum;

		// filepaths
		std::string map_Ka, map_Kd, map_Ks, map_Ns, map_d;
		// normal map
		std::string map_n;
	};
	struct LoadedData
	{
		bool hasTex = false;
		bool hasNorm = false;
		std::string name;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> textureCoords;
		std::vector<glm::vec3> normals;
		std::vector<Face> faces;

		std::string directory;

		LoadedMaterial current_mtl;
		std::unordered_map<std::string, LoadedMaterial> materials;

		std::string current_mtl_name;
		std::string material_file;
	};

	// dont touch
	GLuint materialID = 1;

	GLuint getAndIncrCurrentMatID()
	{
		materialID++;
		return materialID - 1;
	}


	void parseVector(const std::string& line, std::vector<glm::vec3>& vectors)
	{
		std::istringstream ss(line);
		glm::vec3 vec;
		ss >> vec.x;
		ss >> vec.y;
		ss >> vec.z;
		vectors.push_back(vec);
	}

	void parseTextureCoords(const std::string& line, std::vector<glm::vec2>& vectors)
	{
		std::istringstream ss(line);
		glm::vec2 tex;
		ss >> tex.s;
		ss >> tex.t;
		vectors.push_back(tex);
	}

	void parseFace(const std::string & line, LoadedData & data)
	{

		Face face;
		std::istringstream ss(line);

		for (int i = 0; i < 3; i++)
		{
			std::string current;
			ss >> current;

			std::istringstream vecss(current);
			Index index;

			vecss >> index.v;

			std::string tex;
			vecss >> tex;
			if (tex.size() > 0)
			{
				// move past '/'
				tex = tex.substr(1);

				if (tex[0] != '/')
				{
					data.hasTex = true;
					std::string normal;
					std::istringstream texss(tex);
					texss >> index.t;
					texss >> normal;
					if (normal.size() > 0)
					{
						data.hasNorm = true;
						normal = normal.substr(1);
						std::istringstream normss(normal);
						normss >> index.n;
					}
				}
				else
				{
					data.hasNorm = true;
					tex = tex.substr(1);
					std::istringstream normss(tex);
					normss >> index.n;
				}
			}
			face.indices[i] = index;
		}
		face.mtl = data.current_mtl_name;
		data.faces.push_back(face);
	}


	void parseBasicMaterial(const std::string& line, glm::vec3& values)
	{
		std::istringstream ss(line);
		ss >> values.x;
		ss >> values.y;
		ss >> values.z;
	}

	void parseMaterialLine(const std::string & line, LoadedData & data)
	{
		switch (line[0])
		{
		case 'n':
			if (line.substr(0, 6) == "newmtl")
			{
				if (data.current_mtl.name.size() > 0)
				{
					data.current_mtl.id = getAndIncrCurrentMatID();
					data.materials[data.current_mtl.name] = data.current_mtl;
					data.current_mtl = LoadedMaterial();
				}
				data.current_mtl.name = line.substr(7);
			}
		case 'i':
			if (line.substr(0, 5) == "illum")
			{
				std::istringstream ss(line.substr(6));
				ss >> data.current_mtl.illum;
			}
			break;
		case 'K':
			switch (line[1])
			{
			case 'a':
				parseBasicMaterial(line.substr(3), data.current_mtl.Ka);
				break;
			case 'd':
				parseBasicMaterial(line.substr(3), data.current_mtl.Kd);
				break;
			case 's':
				parseBasicMaterial(line.substr(3), data.current_mtl.Ks);
				break;
			}
			break;
		case 'N':
			if (line[1] == 's')
			{
				std::istringstream ss(line.substr(3));
				ss >> data.current_mtl.Ns;
			}
			break;
		case 'd':
		{
			std::istringstream ss(line.substr(2));
			ss >> data.current_mtl.d;
		}
		break;
		case 'T':
			if (line[1] == 'r')
			{
				std::istringstream ss(line.substr(3));
				ss >> data.current_mtl.d;
				data.current_mtl.d = 1 - data.current_mtl.d;
			}
			break;
		case 'm':
			if (line.substr(0, 4) == "map_")
			{
				switch (line[4])
				{
				case 'K':
					switch (line[5])
					{
					case 'a':
						data.current_mtl.map_Ka = line.substr(7);
						break;
					case 'd':
						data.current_mtl.map_Kd = line.substr(7);
						break;
					case 's':
						data.current_mtl.map_Ks = line.substr(7);
						break;
					}
					break;
				case 'N':
					if (line[5] == 's')
					{
						data.current_mtl.map_Ns = line.substr(7);
					}
					break;
				case 'd':
					data.current_mtl.map_d = line.substr(6);
					break;
				case 'n':
					data.current_mtl.map_n = line.substr(6);
					break;
				}
			}
		}
	}


	void loadMaterial(const std::string & filepath, LoadedData & data)
	{
		std::ifstream file;
		file.open(filepath);
		if (file.is_open())
		{
			std::cout << "DEBUG: loaded material: '" << filepath << "'\n";
			std::string line;
			while (std::getline(file, line))
			{
				//std::cout << line << "\n";
				parseMaterialLine(line, data);
			}
			file.close();

			data.current_mtl.id = getAndIncrCurrentMatID();
			data.materials[data.current_mtl.name] = data.current_mtl;
			data.current_mtl = LoadedMaterial();
		}
		else
		{
			std::cerr << "Could not open file: " << filepath << "\n";
		}
		/*
		for (auto m : data.materials)
		{
		auto mat = m.second;
		std::cout << m.first << ":\n";
		std::cout << "\tillum: " << mat.illum << "\n";
		std::cout << "\tKa: " << mat.Ka.x << " " << mat.Ka.y << " " << mat.Ka.z << "\n";
		std::cout << "\tKd: " << mat.Kd.x << " " << mat.Kd.y << " " << mat.Kd.z << "\n";
		std::cout << "\tKs: " << mat.Ks.x << " " << mat.Ks.y << " " << mat.Ks.z << "\n";
		std::cout << "\tTf: " << mat.Tf.x << " " << mat.Tf.y << " " << mat.Tf.z << "\n";
		std::cout << "\tNs: " << mat.Ns << "\n";
		std::cout << "\tNi: " << mat.Ni << "\n";
		std::cout << "\td: " << mat.d << "\n";
		}
		*/
	}

	void parseLine(const std::string& line, LoadedData& data)
	{
		switch (line[0])
		{
		case '#': // Comment
			return;
			break;
		case 'o': // Name
			data.name = line.substr(2);
			break;
		case 'v':
			switch (line[1])
			{
			case ' ': // Vertex
				parseVector(line.substr(2), data.vertices);
				break;
			case 't': // texture coordinate
				parseTextureCoords(line.substr(3), data.textureCoords);
				break;
			case 'n': // Vertex normal
				parseVector(line.substr(3), data.normals);
				break;
			case 'p': // Parameter space vertices ??

				break;
			}
			break;
		case 'f': // polygon face element
			parseFace(line.substr(2), data);
			break;
		case 'm':
			if (line.substr(0, 6) == "mtllib")
			{
				//std::cout << line.substr(7) << "\n";
				loadMaterial(data.directory + line.substr(7), data);
			}
			break;
		case 'u':
			if (line.substr(0, 6) == "usemtl")
			{
				data.current_mtl_name = line.substr(7);
			}
			break;
		}
	}



}



std::shared_ptr<Texture> Assets::loadTextureIfNotLoaded(const std::string & filepath)
{
	std::shared_ptr<Texture> to_add;
	auto search = textures.find(filepath);
	if (search == textures.end()) // not found
	{
		to_add = std::make_shared<Texture>();
		to_add->load(filepath, true);
		textures[filepath] = to_add;
		Log::debugln("Not found. Adding! (" + filepath + ")");
	} 
	return to_add;
}

void Assets::loadMesh(const std::string & filepath)
{
	std::string directory = "";
	size_t last_slash = filepath.rfind('/');
	if (std::string::npos != last_slash)
	{
		directory = filepath.substr(0, last_slash + 1);
	}

	std::shared_ptr<Mesh> result = std::make_shared<Mesh>();
	LoadedData loaded_data;

	loaded_data.directory = directory;
	loaded_data.current_mtl = LoadedMaterial();

	std::ifstream file;
	file.open(filepath);
	if (file.is_open())
	{
		// begin parsing file
		std::string line;
		while (std::getline(file, line))
		{
			parseLine(line, loaded_data);
		}
		file.close();
		// loaded_data is now filled!


		//result.hasNorm = loaded_data.hasNorm;
		//result.hasTex = loaded_data.hasTex;

		for (auto map_elem : loaded_data.materials)
		{
			LoadedMaterial material = map_elem.second;
			Mesh::MeshPart part;
			if (!map_elem.first.empty())
			{
				part.material = std::make_shared<Material>();
				if(!material.map_Kd.empty()) {
					part.material->color = loadTextureIfNotLoaded(directory + material.map_Kd);
				}
				if (!material.map_n.empty())
					part.material->normal = loadTextureIfNotLoaded(directory + material.map_n);
				if (!material.map_Ks.empty())
					part.material->specular = loadTextureIfNotLoaded(directory + material.map_Ks);
				result->addPart(map_elem.first, part);
			}
		}
		int count = 0;
		for (auto face : loaded_data.faces)
		{
			glm::vec3 normal;
			//if (!loaded_data.hasNorm)
			{
				glm::vec3 p1 = loaded_data.vertices[face.indices[0].v - 1];
				glm::vec3 p2 = loaded_data.vertices[face.indices[1].v - 1];
				glm::vec3 p3 = loaded_data.vertices[face.indices[2].v - 1];
				glm::vec3 v1 = p2 - p1;
				glm::vec3 v2 = p3 - p1;
				normal = glm::normalize(glm::cross(v1, v2));
				// choose if smoothe or flat?
			}

			for (int i = 0; i < 3; i++)
			{
				int ind = face.indices[i].v - 1;
				Vertex v;

				v.position = loaded_data.vertices[ind];

				if (loaded_data.hasTex)
				{
					//if (face.indices[i].t - 1 >= 0 || face.indices[i].t - 1 <= loaded_data.textureCoords.size())
					//{
					auto t = loaded_data.textureCoords[face.indices[i].t - 1];
					v.tex_coord = t;
					//}
				}

				if (loaded_data.hasNorm)
				{
					v.normal = loaded_data.normals[face.indices[i].n - 1];;
				}
				else
				{
					v.normal = normal;
				}

				result->vertices.push_back(v);
				result->parts[face.mtl].indices.push_back(count);
				count++;
			}
		}
	}
	meshes[loaded_data.name] = result;
}
