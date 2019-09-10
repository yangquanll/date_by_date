#include<unordered_map>
#include<string>
#include<iostream>

using namespace std;

#define VERIFY_KEYBOX    "kph -c verify_data -n 6 -g keybox"
#define VERIFY_TEE_KEY   "kph -c verify_key -n 0"
#define VERIFY_CONFIG    "kph -c verify_config"
#define GENERATE_TEE_KEY "kph -c generate -n 0"

#define KPH_VERIFY_KEYBOX    0
#define KPH_VERIFY_TEE_KEY   1
#define KPH_VERIFY_CONFIG   2
#define KPH_GENERATE_TEE_KEY 3



int kph_verify_ta_data(uint32_t taid)
{
	cout<< "kph_verify_ta_data"<<endl;
	return KPH_VERIFY_KEYBOX;
}
int kph_verify_ta_key(uint32_t taid)
{
	cout<< "kph_verify_ta_key"<<endl;
	return KPH_VERIFY_TEE_KEY;
}
int kph_verify_device_config(void)
{
	cout<< "kph_verify_device_config"<<endl;
	return KPH_VERIFY_CONFIG;
}
int kph_generate_key_legacy(uint32_t ta_id)
{
	cout<< "kph_generate_key_legacy"<<endl;
	return KPH_GENERATE_TEE_KEY;
}

int invoke_kph_api (int cs)
{
	int ret = 0;
	switch(cs)
	{
		case KPH_VERIFY_KEYBOX: 
			ret = kph_verify_ta_data(6);
		break;
		case KPH_VERIFY_TEE_KEY:
			ret = kph_verify_ta_key(0);
		break;
		case KPH_VERIFY_CONFIG:
			ret = kph_verify_device_config();
		break;
		case KPH_GENERATE_TEE_KEY:
			ret = kph_generate_key_legacy(0);
		break;
		default:
		printf("YQ Unkown cs =",cs);
	}

	return ret;
}

int main (int argc,char *argv[])
{
	string pm;
	int id;
	unordered_map <string ,int> kph_cmd;

	kph_cmd.insert(unordered_map<string, int> ::value_type(VERIFY_KEYBOX, 0));
	kph_cmd.insert(unordered_map<string, int> ::value_type(VERIFY_TEE_KEY, 1));
	kph_cmd.insert(unordered_map<string, int> ::value_type(VERIFY_CONFIG, 2));
	kph_cmd.insert(unordered_map<string, int> ::value_type(GENERATE_TEE_KEY, 3));

	unordered_map <string ,int>::iterator itr;

	if(argc != 0)
	{

		pm = argv[1];
		cout<<"pm = "<<pm<<endl;
		itr = kph_cmd.find(pm);
		if(itr == kph_cmd.end())
		{
			cout<<"not find NULL"<<endl;
		}

		id =itr -> second;
		cout<<"find ID = "<<id<<endl;
	}
    
	invoke_kph_api(id);
}
