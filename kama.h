#include "mecab.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <list>


namespace Kama{

	typedef struct _Node {
		unsigned int id;
		unsigned int nbest;
		char surface[1024];
		char feature[1024*4];
	} Node;
	
	class Tagger{

		private:
			MeCab::Tagger* mcTagger;	
			MeCab::Lattice* mcLattice;
			std::set<std::string> stopwordList;
			Node parseMeCabNode(const MeCab::Node* node, unsigned int nbest);
            const char* defaultDicPath; 
			
			void initialize(const char* dicPath){
				std::string dicPathOption;
                dicPathOption.append(" -d ");
                dicPathOption.append(dicPath);
				this->mcTagger = MeCab::createTagger((const char*)dicPathOption.c_str());
				this->mcLattice = MeCab::createLattice();
				CHECK_ERROR(this->mcTagger);
			}

			void finalize(){
				if(!this->mcTagger){
					delete this->mcTagger;
					this->mcTagger = NULL;
				}
				if(!this->mcLattice){
					delete this->mcLattice;
					this->mcLattice = NULL;
				}
				this->clearStopwordList();
			}

		public:
			Tagger(): mcTagger(NULL), mcLattice(NULL), defaultDicPath("/usr/local/lib/mecab/dic/mecab-ko-dic"){
				this->initialize((const char*)(this->defaultDicPath));
			};

			Tagger(const char* userDicPath){
            	this->initialize(userDicPath);
			};

			~Tagger(){
				this->finalize();		
			};

			std::vector<Kama::Node> parse(const char* str);
			
			std::vector<Kama::Node> parse(const char* str, unsigned int nbest);

			void setStopwordList(std::list<const char*> stopwordList);

			void clearStopwordList();

			void addStopword(const char* stopword);

			void printStopword();

			bool isStopword(const char* word);

			inline void CHECK_ERROR(const void *eval){
				if(!eval){
					const char *e = this->mcTagger ? this->mcTagger->what() : MeCab::getLastError();
					std::cerr << "tagger_exception : " << e << std::endl;
					delete this->mcTagger;
					this->mcTagger = NULL;
				}
			};

	};
	
}
