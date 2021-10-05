#include "Physics/MeshOperation/Delaunay3D.h"
#include "Physics/Basic/Parameter.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <list>

namespace Rocket {
    void Delaunay3D::Initialize(std::vector<VertexPtr>& dots) {
        std::copy(dots.begin(), dots.end(), std::back_inserter(nodes));
		std::mt19937 generator(0);
        std::sort(nodes.begin(), nodes.end(), CompareVertex);
        //std::shuffle(nodes.begin(), nodes.end(), generator);

        elements.clear();
    }

    void Delaunay3D::Generate() {
        MakeMesh(1000, false);
    }

    void Delaunay3D::MakeMesh(int addnodenum, bool iscopynodeexist) {
		//----------Get region which nodes exist----------
		double xmax = 0.0, xmin = 0.0, ymax = 0.0, ymin = 0.0, zmax = 0.0, zmin = 0.0;
		for (auto& node : nodes) {
			if (node->position[0] > xmax) { xmax = node->position[0]; }
			if (node->position[0] < xmin) { xmin = node->position[0]; }
			if (node->position[1] > ymax) { ymax = node->position[1]; }
			if (node->position[1] < ymin) { ymin = node->position[1]; }
			if (node->position[2] > zmax) { zmax = node->position[2]; }
			if (node->position[2] < zmin) { zmin = node->position[2]; }
		}

		//----------Normalize coordinate----------
		double xrange = 0.5*(xmax - xmin), yrange = 0.5*(ymax - ymin), zrange = 0.5*(zmax - zmin);
		double dmax = xrange;
		if (dmax < yrange) { dmax = yrange;	}
		if (dmax < zrange) { dmax = zrange;	}
		for (auto& node : nodes) {
			node->position[0] = (node->position[0] - xmin) / dmax + 0.5*(ALPHA - 1.0)*xrange / dmax;
			node->position[1] = (node->position[1] - ymin) / dmax + 0.5*(ALPHA - 1.0)*yrange / dmax;
			node->position[2] = (node->position[2] - zmin) / dmax + 0.5*(ALPHA - 1.0)*zrange / dmax;
		}

		//----------Make supertetrahedron----------
		double x = ALPHA*xrange / dmax, y = ALPHA*yrange / dmax, z = ALPHA*zrange / dmax;
		MakeSupertetrahedron(x, y, z);

		//----------Make rough mesh----------
		MakeRoughMesh();

		//----------Delete needless elements----------
		DeleteSupertetrahedron();
		if (iscopynodeexist) {
			DeleteCreviceElement();
		}

		//----------Make fine mesh----------
		// MakeFineMesh(addnodenum);

		//----------Renormalize cordinate----------
		for (auto& node : nodes) {
			node->position[0] = node->position[0] * dmax - 0.5*(ALPHA - 1.0)*xrange + xmin;
			node->position[1] = node->position[1] * dmax - 0.5*(ALPHA - 1.0)*yrange + ymin;
			node->position[2] = node->position[2] * dmax - 0.5*(ALPHA - 1.0)*zrange + zmin;
		}

		std::cout << "Final Element Count: " << elements.size() << std::endl;
    }

    void Delaunay3D::MakeSupertetrahedron(double xmax, double ymax, double zmax) {
        std::cout << "Make supertetraedron\n";
		//----------Make nodes of supertetrahedron----------
        VertexPtr nst0 = VertexPtr(new Vertex(0, 0, 0, -1));
        VertexPtr nst1 = VertexPtr(new Vertex(xmax, 0, 0, -1));
		VertexPtr nst2 = VertexPtr(new Vertex(xmax, ymax, 0, -1));
		VertexPtr nst3 = VertexPtr(new Vertex(0, ymax, 0, -1));
		VertexPtr nst4 = VertexPtr(new Vertex(0, 0, zmax, -1));
		VertexPtr nst5 = VertexPtr(new Vertex(xmax, 0, zmax, -1));
		VertexPtr nst6 = VertexPtr(new Vertex(xmax, ymax, zmax, -1));
		VertexPtr nst7 = VertexPtr(new Vertex(0, ymax, zmax, -1));
		nodes.push_back(nst0);
		nodes.push_back(nst1);
		nodes.push_back(nst2);
		nodes.push_back(nst3);
		nodes.push_back(nst4);
		nodes.push_back(nst5);
		nodes.push_back(nst6);
		nodes.push_back(nst7);
		
		//----------Make elements of supertetrahedron----------
		elements.emplace_back(new Tetrahedra(nst1, nst3, nst0, nst7));
		elements.emplace_back(new Tetrahedra(nst2, nst1, nst6, nst7));
		elements.emplace_back(new Tetrahedra(nst2, nst3, nst1, nst7));
		elements.emplace_back(new Tetrahedra(nst1, nst5, nst6, nst7));
		elements.emplace_back(new Tetrahedra(nst1, nst0, nst5, nst7));
		elements.emplace_back(new Tetrahedra(nst4, nst5, nst0, nst7));

		//----------Make connection of supertetrahedron----------
		for (auto& pelement : elements) {
			for (auto& psurface : pelement->faces) {
				if (psurface->neighbor != nullptr) continue;
				for (auto& pelement2 : elements) {
					if(pelement2 == pelement) continue;
					for (auto& psurface2 : pelement2->faces) {
						if (psurface->IsCoincidentWith(psurface2.get())) {
							psurface->neighbor = pelement2.get();
							psurface2->neighbor = pelement.get();
							break;
						}
					}
				}
			}
		}
    }

    void Delaunay3D::MeshLocal(VertexPtr& node, Tetrahedra*& ethis) {
        std::vector<Tetrahedra*> stack, substack;
		std::vector<Triangle*> sstack;

		//----------Get elements which node is in----------
		substack.push_back(ethis);
		while (substack.size()) {
			Tetrahedra* pend = substack.back();			
			substack.pop_back();

			if (pend->is_active) {
				stack.push_back(pend);
				pend->is_active = false;

				for (auto& surface : pend->faces) {
					Tetrahedra* pneighbor = surface->neighbor;
					if (pneighbor != nullptr && pneighbor->IsInSphere(node)) {
						substack.push_back(pneighbor);
					} else {
						sstack.push_back(surface.get());
					}
				}
			}
		}
		//std::cout << "Get elements stack Size : " << stack.size() << std::endl;
		//std::cout << "Get elements substack Size : " << substack.size() << std::endl;
		//std::cout << "Get elements sstack Size : " << sstack.size() << std::endl;

		//----------Modify crevice of polyhedron----------
		bool is_anysurface_invalid = true;
		while (is_anysurface_invalid) {
			is_anysurface_invalid = false;

			for (int i = 0; i < sstack.size(); i++) {
				if (sstack[i]->is_active) {
					Tetrahedra D = Tetrahedra(sstack[i]->vertices[0], sstack[i]->vertices[1], sstack[i]->vertices[2], node);
					
					//----------if there are crevices----------
					if (D.volume < EPS) {
						Tetrahedra* peadd = sstack[i]->neighbor;		

						//----------if able to add elements----------
						if (peadd != nullptr) {
							if (peadd->is_active) {
								is_anysurface_invalid = true;
								peadd->is_active = false;
								stack.push_back(peadd);
								
								//----------make surfaces isactive false----------
								for (auto& surface : peadd->faces) {
									Tetrahedra* pneighbor = surface->neighbor;
									if (pneighbor != nullptr && !pneighbor->is_active) {
										pneighbor->GetAdjacentSurface(peadd)->is_active = false;
									} else {
										sstack.push_back(surface.get());
									}
								}
								break;
							}
						} else if (fabs(D.volume) < EPS) {
							sstack[i]->is_active = false;
						}
					}
				}
			}
		}
		//std::cout << "Modify crevice stack Size : " << stack.size() << std::endl;
		//std::cout << "Modify crevice substack Size : " << substack.size() << std::endl;
		//std::cout << "Modify crevice sstack Size : " << sstack.size() << std::endl;

		//----------Make new elements----------
		//std::cout << "Before Insert Size : " << elements.size() << std::endl;
		std::vector<TetrahedraPtr> penew;			
		for (auto& psurface : sstack) {
			if (psurface->is_active) {
				TetrahedraPtr tmp = TetrahedraPtr(new Tetrahedra(psurface->vertices[0], psurface->vertices[1], psurface->vertices[2], node));
				tmp->faces[3]->neighbor = psurface->neighbor;
				if (psurface->neighbor != nullptr) {
					psurface->neighbor->GetAdjacentSurface(psurface->parent)->neighbor = tmp.get();
				}
				penew.push_back(elements.back());
                elements.push_back(tmp);
				//temp_storage.push_back(tmp);
			}
		}
		//std::cout << "After Insert Size : " << elements.size() << std::endl;

		//----------Make connection of new elements----------
		for (auto& pelement : penew) {
			for (auto& psurface : pelement->faces) {
				bool is_out = false;
				if (psurface->neighbor == nullptr) {
					for (auto& pelement2 : penew) {
						for (auto& psurface2 : pelement2->faces) {
							if (psurface == psurface2) {

								//----------if invalid element is made----------
								if (psurface2->neighbor != nullptr) {
                                    std::cout << "!!\n";
								}

								psurface->neighbor = pelement2.get();
								psurface2->neighbor = pelement.get();
								is_out = true;
								break;
							}
						}
						if(is_out) break;
					}
				}
			}
		}

		//----------Delete needless elements in stack----------
		//std::cout << "Before Delete Size : " << elements.size() << std::endl;
		for (auto pelement = elements.begin(); pelement != elements.end(); ) {
			if (!(*pelement)->is_active) {
				pelement = elements.erase(pelement);
			} else {
				++pelement;
			}
		}
		//std::cout << "After Delete Size : " << elements.size() << std::endl;
    }

	void Delaunay3D::StandardMethod(VertexPtr& pnode, std::unordered_map<int32_t, TrianglePtr>& faces) {
		// Origin Method Slow
		for(auto pelement = elements.begin(); pelement != elements.end();) {
			//----------if node is in the element----------
			if((*pelement)->IsInSphere(pnode)) {
				for(auto pface : (*pelement)->faces) {
					bool find = false;
					for(auto face_find = faces.begin(); face_find != faces.end();) {
						if((*face_find).second->id == pface->id) {
							face_find = faces.erase(face_find);
							find = true;
						}
						else if((*face_find).second->IsCoincidentWith((pface).get())) {
							face_find = faces.erase(face_find);
							find = true;
						}
						else {
							face_find++;
						}
					}
					if(!find) {
						faces[pface->id] = pface;
					}
				}
				pelement = elements.erase(pelement);
			}
			else {
				pelement++;
			}
		}

		for(auto& face_pair : faces) {
			auto& pface = face_pair.second;
			TetrahedraPtr tmp = TetrahedraPtr(new Tetrahedra(pface->vertices[0], pface->vertices[1], pface->vertices[2], pnode));
			elements.push_back(tmp);
		}
	}

	Tetrahedra* Delaunay3D::FastMethod(VertexPtr& pnode, Tetrahedra* pethis) {
		// Unknown Method Fast
		while (1) {
			Tetrahedra* penext = pethis->GetLocateId(pnode);				
			//----------if node is in the element----------
			if (penext == pethis) {
				MeshLocal(pnode, pethis);
				pethis = elements.back().get();
				break;
			}
			else {
				pethis = penext;
			}
		}
		return pethis;
	}

    void Delaunay3D::MakeRoughMesh() {
        std::cout << "Make rough mesh\n";

		std::unordered_map<int32_t, TrianglePtr> faces;
        Tetrahedra* pethis = elements[0].get();								
		for (auto& pnode : nodes) {
			if (pnode->type != -1) {
				if(method == 1) {
					StandardMethod(pnode, faces);
				}
				else if(method == 2) {
					pethis = FastMethod(pnode, pethis);
				}
			}
		}
    }

    void Delaunay3D::DeleteSupertetrahedron() {
        std::cout << "Delete supertetraedron\n";
		
		for (auto pelement = elements.begin(); pelement != elements.end(); ) {
			if ((*pelement)->nodes[0]->type == -1 
				|| (*pelement)->nodes[1]->type == -1 
				|| (*pelement)->nodes[2]->type == -1 
				|| (*pelement)->nodes[3]->type == -1) {
				for (auto& psurface : (*pelement)->faces) {
					if ((psurface)->neighbor != nullptr) {
						(psurface)->neighbor->GetAdjacentSurface(psurface->parent)->neighbor = nullptr;
					}
				}				
				pelement = elements.erase(pelement);
			} else {
				++pelement;
			}
        }

		for (auto pnode = nodes.begin(); pnode != nodes.end(); ) {
			if((*pnode)->type == -1) {
				nodes.erase(pnode);
			} else {
				++pnode;
			}
		}
    }

    void Delaunay3D::DeleteCreviceElement() {
        std::cout << "Delete Crevice Element\n";

		for (auto pelement = elements.begin(); pelement != elements.end(); ) {
			if ((*pelement)->nodes[0]->type == (*pelement)->nodes[1]->type
				&& (*pelement)->nodes[1]->type == (*pelement)->nodes[2]->type
				&& (*pelement)->nodes[2]->type == (*pelement)->nodes[3]->type) {
				for (auto& psurface : (*pelement)->faces) {
					if (psurface->neighbor != nullptr) {
						psurface->neighbor->GetAdjacentSurface(psurface->parent)->neighbor = nullptr;
					}
				}
				pelement = elements.erase(pelement);
			}
			else {
				++pelement;
			}
		}
    }

    void Delaunay3D::MakeFineMesh(int addnodenum) {
        std::cout << "Make fine mesh\n";
		
		for (int i = 0; i < addnodenum; i++) {
			//----------Find element which has longest edge----------
			double edgelengthmax = 0;
			Tetrahedra* pethis = nullptr;
			VertexPtr pnode0 = nullptr;
			VertexPtr pnode1 = nullptr;

			for (auto& pelement : elements) {
				for (int j = 0; j < 3; j++) {
					for (int k = j + 1; k < 3; k++) {
						double edgelength = (pelement->nodes[k]->position - pelement->nodes[j]->position).norm();
						if (edgelength > edgelengthmax) {
							edgelengthmax = edgelength;
							pethis = pelement.get();
							pnode0 = pelement->nodes[j];
							pnode1 = pelement->nodes[k];
						}
					}
				}
			}

			//----------Add Node----------
			if(pnode0.get() != nullptr && pnode1.get() != nullptr) {
				Eigen::Vector3d target = (pnode0->position + pnode1->position) / 2.0;
				VertexPtr tmp = VertexPtr(new Vertex(target, 2));
				nodes.push_back(tmp);
				MeshLocal(tmp, pethis);
			}
		}
    }
}
