#include "Physics/MeshOperation/Delaunay3D.h"
#include "Physics/BasicElement/Parameter.h"

#include <iostream>

namespace Rocket {
    void Delaunay3D::Initialize(std::vector<Vertex>& dots) {
        std::copy(dots.begin(), dots.end(), std::back_inserter(nodes));
        elements.clear();
    }

    void Delaunay3D::Generate() {
        MakeMesh(50000, true);
    }

    void Delaunay3D::MakeMesh(int addnodenum, bool iscopynodeexist) {
        //----------Get region which nodes exist----------
		double xmax = 0.0, xmin = 0.0, ymax = 0.0, ymin = 0.0, zmax = 0.0, zmin = 0.0;
		for (auto node : nodes) {
			if (node.position[0] > xmax) { xmax = node.position[0]; }
			if (node.position[0] < xmin) { xmin = node.position[0]; }
			if (node.position[1] > ymax) { ymax = node.position[1]; }
			if (node.position[1] < ymin) { ymin = node.position[1]; }
			if (node.position[2] > zmax) { zmax = node.position[2]; }
			if (node.position[2] < zmin) { zmin = node.position[2]; }
		}

		//----------Normalize coordinate----------
		double xrange = 0.5*(xmax - xmin), yrange = 0.5*(ymax - ymin), zrange = 0.5*(zmax - zmin);
		double dmax = xrange;
		if (dmax < yrange) { dmax = yrange;	}
		if (dmax < zrange) { dmax = zrange;	}
		for (auto& node : nodes) {
			node.position[0] = (node.position[0] - xmin) / dmax + 0.5*(ALPHA - 1.0)*xrange / dmax;
			node.position[1] = (node.position[1] - ymin) / dmax + 0.5*(ALPHA - 1.0)*yrange / dmax;
			node.position[2] = (node.position[2] - zmin) / dmax + 0.5*(ALPHA - 1.0)*zrange / dmax;
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
			node.position[0] = node.position[0] * dmax - 0.5*(ALPHA - 1.0)*xrange + xmin;
			node.position[1] = node.position[1] * dmax - 0.5*(ALPHA - 1.0)*yrange + ymin;
			node.position[2] = node.position[2] * dmax - 0.5*(ALPHA - 1.0)*zrange + zmin;
		}
    }

    void Delaunay3D::MakeSupertetrahedron(double xmax, double ymax, double zmax) {
        std::cout << "Make supertetraedron\n";
		//----------Make nodes of supertetrahedron----------
		nodes.emplace_back(0, 0, 0, -1);
        Vertex& nst0 = nodes.back();
		nodes.emplace_back(xmax, 0, 0, -1);
        Vertex& nst1 = nodes.back();
		nodes.emplace_back(xmax, ymax, 0, -1);
		Vertex& nst2 = nodes.back();
		nodes.emplace_back(0, ymax, 0, -1);
		Vertex& nst3 = nodes.back();
		nodes.emplace_back(0, 0, zmax, -1);
		Vertex& nst4 = nodes.back();
		nodes.emplace_back(xmax, 0, zmax, -1);
		Vertex& nst5 = nodes.back();
		nodes.emplace_back(xmax, ymax, zmax, -1);
		Vertex& nst6 = nodes.back();
		nodes.emplace_back(0, ymax, zmax, -1);
		Vertex& nst7 = nodes.back();
		
		//----------Make elements of supertetrahedron----------
		elements.emplace_back(std::make_shared<Tetrahedra>(nst1, nst3, nst0, nst7));
		elements.emplace_back(std::make_shared<Tetrahedra>(nst2, nst1, nst6, nst7));
		elements.emplace_back(std::make_shared<Tetrahedra>(nst2, nst3, nst1, nst7));
		elements.emplace_back(std::make_shared<Tetrahedra>(nst1, nst5, nst6, nst7));
		elements.emplace_back(std::make_shared<Tetrahedra>(nst1, nst0, nst5, nst7));
		elements.emplace_back(std::make_shared<Tetrahedra>(nst4, nst5, nst0, nst7));

		//----------Make connection of supertetrahedron----------
		for (auto& pelement : elements) {
			for (auto& psurface : pelement->faces) {
				if (psurface.neighbor == nullptr) {
					for (auto& pelement2 : elements) {
						for (auto& psurface2 : pelement2->faces) {
							if (psurface == psurface2) {
								psurface.neighbor = pelement2;
								psurface2.neighbor = pelement;
								break;
							}
						}
					}
				}
			}
		}
    }

    void Delaunay3D::MeshLocal(Vertex* node, std::shared_ptr<Tetrahedra>& ethis) {
        std::vector<std::shared_ptr<Tetrahedra>> stack, substack;
		std::vector<Triangle*> sstack;

		//----------Get elements which node is in----------
		substack.push_back(ethis);
		while (substack.size()) {
			std::shared_ptr<Tetrahedra> pend = substack.back();			
			substack.pop_back();

			if (pend->is_active) {
				stack.push_back(pend);
				pend->is_active = false;

				for (auto& surface : pend->faces) {
					std::shared_ptr<Tetrahedra> pneighbor = surface.neighbor;
					if (pneighbor != nullptr && pneighbor->IsInSphere(*node)) {
						substack.push_back(pneighbor);
					} else {
						sstack.push_back(&surface);
					}
				}
			}
		}

		//----------Modify crevice of polyhedron----------
		bool is_anysurface_invalid = true;
		while (is_anysurface_invalid) {
			is_anysurface_invalid = false;

			for (int i = 0; i < sstack.size(); i++) {
				if (sstack[i]->is_active) {
					Tetrahedra D = Tetrahedra(sstack[i]->vertices[0], sstack[i]->vertices[1], sstack[i]->vertices[2], *node);
					
					//----------if there are crevices----------
					if (D.volume < EPS) {
						std::shared_ptr<Tetrahedra> peadd = sstack[i]->neighbor;		

						//----------if able to add elements----------
						if (peadd != nullptr) {
							if (peadd->is_active) {
								is_anysurface_invalid = true;
								peadd->is_active = false;
								stack.push_back(peadd);
								
								//----------make surfaces isactive false----------
								for (auto& surface : peadd->faces) {
									std::shared_ptr<Tetrahedra>& pneighbor = surface.neighbor;
									if (pneighbor != nullptr && !pneighbor->is_active) {
										pneighbor->GetAdjacentSurface(peadd)->is_active = false;
									} else {
										sstack.push_back(&surface);
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

		//----------Make new elements----------
		std::vector<std::shared_ptr<Tetrahedra>> penew;			
		for (auto& surface : sstack) {
			if (surface->is_active) {
				auto& tmp = std::make_shared<Tetrahedra>(surface->vertices[0], surface->vertices[1], surface->vertices[2], *node);
				tmp->faces[3].neighbor = surface->neighbor;
				if (surface->neighbor != nullptr) {
					surface->neighbor->GetAdjacentSurface(surface->parent)->neighbor = tmp;
				}
				penew.push_back(elements.back());
                elements.push_back(tmp);
			}
		}

		//----------Make connection of new elements----------
		for (auto& pelement : penew) {
			for (auto& psurface : pelement->faces) {
				OUT:
				if (psurface.neighbor == nullptr) {
					for (auto& pelement2 : penew) {
						for (auto& psurface2 : pelement2->faces) {
							if (psurface == psurface2) {

								//----------if invalid element is made----------
								if (psurface2.neighbor != nullptr) {
                                    std::cout << "!!\n";
								}

								psurface.neighbor = pelement2;
								psurface2.neighbor = pelement;
								goto OUT;
							}
						}
					}
				}
			}
		}

		//----------Delete needless elements in stack----------
		for (auto pelement = elements.begin(); pelement != elements.end(); ) {
			if (!(*pelement)->is_active) {
				pelement = elements.erase(pelement);
			} else {
				++pelement;
			}
		}
    }

    void Delaunay3D::MakeRoughMesh() {
        std::cout << "Make rough mesh\n";

        std::shared_ptr<Tetrahedra> pethis = elements[0];									
		for (auto& pnode : nodes) {
			if (pnode.type != -1) {
				int count = 0;
				while (1) {
					std::shared_ptr<Tetrahedra> penext = pethis->GetLocateId(pnode);				
					//----------if node is in the element----------
					if (*penext.get() == *pethis.get() || penext.get() == pethis.get()) {
						MeshLocal(&pnode, pethis);
						pethis = elements.back();
						break;
					} else {
						pethis = penext;
					}
				}
			}
		}
    }

    void Delaunay3D::DeleteSupertetrahedron() {
        std::cout << "Delete supertetraedron\n";
		
		for (auto pelement = elements.begin(); pelement != elements.end(); ) {
			if ((*pelement)->nodes[0].type == -1 
				|| (*pelement)->nodes[1].type == -1 
				|| (*pelement)->nodes[2].type == -1 
				|| (*pelement)->nodes[3].type == -1) {
				for (auto& psurface : (*pelement)->faces) {
					if ((psurface).neighbor != nullptr) {
						(psurface).neighbor->GetAdjacentSurface(psurface.parent)->neighbor = nullptr;
					}
				}				
				pelement = elements.erase(pelement);
			} else {
				++pelement;
			}
        }
    }

    void Delaunay3D::DeleteCreviceElement() {
        std::cout << "Delete Crevice Element\n";

		for (auto pelement = elements.begin(); pelement != elements.end(); ) {
			if ((*pelement)->nodes[0].type == (*pelement)->nodes[1].type
				&& (*pelement)->nodes[1].type == (*pelement)->nodes[2].type
				&& (*pelement)->nodes[2].type == (*pelement)->nodes[3].type) {
				for (auto& psurface : (*pelement)->faces) {
					if (psurface.neighbor != nullptr) {
						psurface.neighbor->GetAdjacentSurface(psurface.parent)->neighbor = nullptr;
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
			std::shared_ptr<Tetrahedra> pethis = nullptr;
			Vertex* pnode0 = nullptr;
			Vertex* pnode1 = nullptr;

			for (auto& pelement : elements) {
				for (int j = 0; j < 3; j++) {
					for (int k = j + 1; k < 3; k++) {
						double edgelength = (pelement->nodes[k].position - pelement->nodes[j].position).norm();
						if (edgelength > edgelengthmax) {
							edgelengthmax = edgelength;
							pethis = pelement;
							pnode0 = &pelement.get()->nodes[j];
							pnode1 = &pelement.get()->nodes[k];
						}
					}
				}
			}

			//----------Add Node----------
			nodes.emplace_back(((*pnode0).position + (*pnode1).position) / 2.0);
			Vertex& tmp = nodes.back();
			tmp.type = 2;
			MeshLocal(&tmp, pethis);
		}
    }
}
