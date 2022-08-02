#include "Trenchbroom.h"

void Trenchbroom::LoadMAP(std::string url)
{
	std::vector<std::string> lines = Files::ReadAndGetLines(url.c_str());

	bool isInWorldSpawn = false;
	bool isInBrush = false;

	for(auto i : lines)
	{
		if(StringAPI::StartsWith(i, "//"))
		{
			continue;
		}

		//std::cout << i << std::endl;
		if(WorldSpawnFound(i))
		{
			isInWorldSpawn = true;
			//std::cout << "Opening World Spawn..." << std::endl;
		}

		if(OpenBracketFound(i) && isInWorldSpawn)
		{
			isInBrush = true;
			//std::cout << "Opening Brush..." << std::endl;

			brushes.push_back({});
		}

		if(CloseBracketFound(i) && isInWorldSpawn && isInBrush)
		{
			isInBrush = false;
			//std::cout << "Closing Brush..." << std::endl;
		}
		else if(CloseBracketFound(i) && !isInBrush)
		{
			isInWorldSpawn = false;
			//std::cout << "Closing World Spawn..." << std::endl;
		}


		if(isInWorldSpawn && isInBrush)
		{
			Face f;
			if(BrushInfoFound(i))
			{
				//std::cout << "Brush Info Found!\n";
				i = StringAPI::ReplaceAll(i, "( ", "(");
				i = StringAPI::ReplaceAll(i, " )", ")");

				int vectorsParsed = 0;
				while(vectorsParsed != 3)
				{
					std::string s = StringAPI::GetSubstringBetween(i, "(", ")");

					s = "(" + s;
					s = s + ")";

					std::string res = s;
					res = StringAPI::ReplaceAll(res, "(", "[");
					res = StringAPI::ReplaceAll(res, ")", "]");
					res = StringAPI::ReplaceAll(res, " ", "|");

					i = StringAPI::ReplaceAll(i, s, res);
					vectorsParsed++;
				}

				std::vector<std::string> split = StringAPI::SplitIntoVector(i, " ");
				int vectorCount = 0;
				for(auto ii : split)
				{
					//std::cout << ii << std::endl;

					if(StringAPI::StartsWith(ii, "["))
					{
						std::string valueString = StringAPI::GetSubstringBetween(ii, "[", "]");
						std::vector<std::string> vals = StringAPI::SplitIntoVector(valueString, "|");
						Vector3 result = Vector3(std::stof(vals[0]), std::stof(vals[1]), std::stof(vals[2]));
	
						if(vectorCount == 0)
							f.a1 = result;
						else if(vectorCount == 1)
							f.b2 = result;
						else if(vectorCount == 2)
							f.c3 = result;
					}

					vectorCount++;
				}
				
				brushes[brushes.size() - 1].faces.push_back(f);
				//std::cout << "Face Sent! " << f.a1.ToString() << " " << f.b2.ToString() << " " << f.c3.ToString() << "\n";
			}
		}
	}
}

Model* Trenchbroom::MAPToModel()
{
	std::vector<float> v;
	for(auto b : brushes)
	{
		//std::cout << "Start New Brush!\n";
		//std::vector<std::array<float, 4>> firstVertex;
		for(auto f : b.faces)
		{
			std::vector<std::array<float, 4>> faceVertices;
			int interS = 0;
			Plane p = Plane(f.a1, f.b2, f.c3);
			Vector3 pXYZ = Vector3(p.GetNormal().x, p.GetNormal().y, p.GetNormal().z);
			for(auto f2 : b.faces)
			{
				Plane p2 = Plane(f2.a1, f2.b2, f2.c3);
				Vector3 p2XYZ = Vector3(p2.GetNormal().x, p2.GetNormal().y, p2.GetNormal().z);
				for(auto f3 : b.faces)
				{
					Plane p3 = Plane(f3.a1, f3.b2, f3.c3);
					Vector3 p3XYZ = Vector3(p3.GetNormal().x, p3.GetNormal().y, p3.GetNormal().z);
					glm::vec3 res;

					Vector3 n2n3 = Vector3::Cross(p2XYZ, p3XYZ);
                	Vector3 n3n1 = Vector3::Cross(p3XYZ, pXYZ);
                	Vector3 n1n2 = Vector3::Cross(pXYZ, p2XYZ);

                	if(n2n3.LengthSquared() <= 0.1f ||
                		n3n1.LengthSquared() <= 0.1f ||
                		n1n2.LengthSquared() <= 0.1f)
                	{
                		continue;
                	}

                	float quotient = Vector3::Dot(pXYZ, n2n3);

                	if(std::fabs(quotient) <= 0.1f)
                	{
                		continue;
                	}

                	quotient = -1.0f / quotient;
                	n2n3 *= -p.GetDistance();
                	n3n1 *= -p2.GetDistance();
                	n1n2 *= -p3.GetDistance();
                	Vector3 potentialVertex = n2n3 + n3n1 + n1n2;
                	potentialVertex *= quotient;
									glm::vec3 tmp(potentialVertex.x, potentialVertex.y, potentialVertex.z);
                	if (!Plane::IsVectorInsidePlanes(b.faces, tmp))
                    	continue;

                    std::array<float, 4> ye;
                    ye[0] = potentialVertex.y / 16.0f;
                    ye[1] = potentialVertex.z / 16.0f;
                    ye[2] = potentialVertex.x / 16.0f;
                    ye[3] = 1.0f;

                    bool isRepeated = false;
                    for(auto v : faceVertices)
                    {
                    	if(v == ye)
                    	{
                    		isRepeated = true;
                    		break;
                    	}
                    }

                    if(isRepeated)
                    	continue;

                    faceVertices.push_back(ye);
				}
			}

			if(faceVertices.size() != 0)
			{
				std::vector<std::array<float, 4>> resFaceVertices;
				
				QuakeFaceDictionary::Order(faceVertices, resFaceVertices, faceVertices.size());

				if(faceVertices.size() > 4)
				{
					std::cout << "==============\n";
					std::cout << "WEIRD FACE!\n";
					for(int i = 0; i < faceVertices.size(); i++)
					{
						std::cout << "(" << faceVertices[i][0] << ", " << faceVertices[i][1] << ", " << faceVertices[i][2] << ") IS: " << i << "\n";
					}
					std::cout << "==============\n";
				}
	
				for(auto ii : resFaceVertices)
				{
					for(int e = 0; e < 4; e++)
					{
						v.push_back(ii[e]);
					}
				}
			}
		}
	}
	//std::cout << "Model Completed! Size: " << v.size() << std::endl; 
	return new Model(v, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
}

void QuakeFaceDictionary::Order(std::vector<std::array<float, 4>>& quake, std::vector<std::array<float, 4>>& res, int amount)
{
	for(int i = 0; i < quake.size(); i++)
	{
		if(i + 2 < quake.size())
		{
			res.push_back(quake[i]);
			res.push_back(quake[i + 1]);
			res.push_back(quake[i + 2]);
			res.push_back(quake[i + 2]);
		}
	}
}
