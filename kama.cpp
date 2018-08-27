#include "kama.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <fstream>

namespace Kama{

	Node Tagger::parseMeCabNode(const MeCab::Node* node, unsigned int nbest){
		Node tNode;
		tNode.surface[0] = '\0';
		tNode.feature[0] = '\0';
		if(node->length == 0){
			return tNode;
		}
		// tNode.surface = (char*)malloc(sizeof(char)*(node->length+1));
		strncpy(tNode.surface, node->surface, node->length);
		tNode.surface[node->length] = '\0';
		if(isStopword(tNode.surface)){
			tNode.surface[0] = '\0';
			return tNode;
		}
		//tNode.feature = (char*)(node->feature);
		strcpy(tNode.feature, node->feature);
		tNode.nbest = nbest;
		tNode.id = node->id;

		return tNode;
	}

	std::vector<Kama::Node> Tagger::parse(const char* str){
		return parse(str, 1);
	}

	std::vector<Kama::Node> Tagger::parse(const char* str, unsigned int nbest){
		std::vector<Kama::Node> nodeVector;
		if(str == NULL){
			return nodeVector;
		}

		this->mcLattice->set_request_type(MECAB_NBEST);
		this->mcLattice->set_sentence(str);
		this->mcTagger->parse(this->mcLattice);

		unsigned int tNbest = 1;
		for(;tNbest <= nbest;){
			if(!(this->mcLattice->next())){
				break;
			}
			MeCab::Node* node = this->mcLattice->bos_node();
			CHECK_ERROR(node);
			for(; node; node = node->next) {
				if(node->stat == MECAB_EOS_NODE){
					tNbest++;
					continue;
				}
				Kama::Node tNode = parseMeCabNode(node, tNbest);
				if(strlen(tNode.surface) == 0){
					continue;
				}
				nodeVector.push_back(tNode);
			}	
		}
		this->mcLattice->clear();
		return nodeVector;
	}

	bool Tagger::isStopword(const char* word){
		bool ret = this->stopwordList.find((std::string)word) != this->stopwordList.end();
		return ret;
	}

	void Tagger::printStopword(){
		for(std::set<std::string>::const_iterator i = this->stopwordList.begin(); i != this->stopwordList.end(); i++){
			std::cout << "prn" << "(" << (*i).length() << ") : "<< *i << std::endl;
		}
	}

	void Tagger::setStopwordList(std::list<const char*> stopwordList){
		clearStopwordList();
		for(std::list<const char*>::const_iterator i = stopwordList.begin(); i != stopwordList.end(); i++){
			this->stopwordList.insert((std::string)(*i));
		}
		return;
	}

	void Tagger::clearStopwordList(){
		for(std::set<std::string>::const_iterator i = this->stopwordList.begin(); i != this->stopwordList.end(); i++){
			this->stopwordList.erase(i);
		}
		this->stopwordList.clear();
	}

	void Tagger::addStopword(const char* stopword){
		this->stopwordList.insert((std::string)(stopword));
		return;
	}

}

int main(int argc, char** argv)
{
	Kama::Tagger* tagger = new Kama::Tagger();
	if(argc < 2){
		std::cout << "usage : " << argv[0] << " [parse string]" << std::endl;
		exit(0);
	}
	std::vector<Kama::Node> nodeVector = tagger->parse((const char*)(argv[1]));
	for(std::vector<Kama::Node>::const_iterator i = nodeVector.begin(); i != nodeVector.end(); i++){
		std::cout << "[" << i->nbest << "] : " << i->id << ' ' << i->surface << ' ' << i->feature << std::endl;
	}
	delete tagger;
	return 0;
}
