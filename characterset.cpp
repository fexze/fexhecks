#include <string.h>
#include "characterset.hpp"

void CharacterSetBuild(characterset_t* pSetBuffer, const char* pszSetString)
{
    int i = 0;

    if(!pSetBuffer || !pszSetString)
        return;

    memset(pSetBuffer->Set, 0, sizeof(pSetBuffer->Set));

    while(pszSetString[i]) 
	{
        pSetBuffer->Set[pszSetString[i]] = 1;
        i++;
    }
}