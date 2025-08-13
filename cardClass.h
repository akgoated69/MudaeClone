#ifndef cardClass
#define cardClass
#include <string>

namespace Mudae
{
    class Card
    {
        public:
        int id;
        std::string imgurLink;
        Card(int idTemp, std::string i)
        {
            id = idTemp; 
            imgurLink = i;
        }
        Card()
        {
            id = 0;
            imgurLink = "";
        }
        void setCard(int idTemp, std::string i)
        {
            id = idTemp;
            imgurLink = i;
        }
    };
}

#endif