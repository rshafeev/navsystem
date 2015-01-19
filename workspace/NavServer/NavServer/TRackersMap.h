#pragma once

#include <hash_map>
#include "DispatcherClientHandler.h"

template< > inline

size_t stdext::hash_value< CString >(const CString & s)
{
	return stdext::hash_value((LPCTSTR)s);

}

class CTrackersMap
{
	hash_map<CString, vector<CDispatcherClientHandler*>> trackersMap;
public:
	CTrackersMap();
	~CTrackersMap();

	vector<CDispatcherClientHandler*> findDiscepthersListByTracker(CString IMEI)
	{
		hash_map <CString, vector<CDispatcherClientHandler*>> ::const_iterator i = trackersMap.find(IMEI);
		if (i == trackersMap.end())
		{
			typedef pair <CString, vector<CDispatcherClientHandler*>> Map_Pair;
			vector<CDispatcherClientHandler*> arr;
			trackersMap.insert(Map_Pair(IMEI, arr));
			return arr;
		}
		return i->second;
	}

	bool isExists(CString IMEI, CDispatcherClientHandler& client)
	{
		vector<CDispatcherClientHandler*> clients = findDiscepthersListByTracker(IMEI);
		for (unsigned int i = 0; i < clients.size(); i++)
		{
			if (clients[i] != nullptr && clients[i]->Client().ID() == client.Client().ID())
				return true;

		}
		return false;
	}
	void addDispetcherToTrackersMap(CString IMEI, CDispatcherClientHandler& client)
	{
		if (isExists(IMEI, client))
		{
			return;
		}
		vector<CDispatcherClientHandler*> clients = findDiscepthersListByTracker(IMEI);
		clients.push_back(&client);
	}

	void removeClient(CAbstractClientHandler* client)
	{
		if (client == nullptr)
			return;
		if (client->Type() == ClientType::DISPETCHER)
		{
			hash_map <CString, vector<CDispatcherClientHandler*>> ::const_iterator i;
			for (i = this->trackersMap.begin(); i != this->trackersMap.end(); ++i)
			{
				vector<CDispatcherClientHandler*> clients = (*i).second;
				std::vector<CDispatcherClientHandler*>::iterator j = clients.begin();
				while (j != clients.end())
				{
					if (*j == client){
						clients.erase(j);
						break;
					}
					++j;
				}
			}
		}
	}

};

