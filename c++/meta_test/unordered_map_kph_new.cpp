#include<unordered_map>
#include<string>
#include<iostream>
#include <string.h>

using namespace std;

#define VERIFY_KEYBOX    "kph -c verify_data -n 6 -g keybox"
#define VERIFY_TEE_KEY   "kph -c verify_key -n 0"
#define VERIFY_CONFIG    "kph -c verify_config"
#define GENERATE_TEE_KEY "kph -c generate -n 0"

typedef enum candid
{
	KPH_GENERATE = 0,
	KPH_VERIFY_CONFIG,
	KPH_VERIFY_KEY,
	KPH_VERIFY_DATA,
	KPH_UPDATE_PSTATE,
}ID_CANDID;

#if 0
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
#endif
static char * parser_cmd_buf( char *cmdbuf,int &typ, int &num)
{
	uint32_t i,x;
	char *out_cmd_head;
	const char *candidate_cmd_head[] = { "generate", "verify_config", "verify_key", "verify_data", "update_pstate" };// 5                                                                                                                                 
	const char *candidate_num_head[] = {"6", "0"};

	cout<<"input buf = "<<cmdbuf<<endl;
	for (i = 0; i < sizeof(candidate_cmd_head) / sizeof(candidate_cmd_head[0]); i++) {
		if (strstr(cmdbuf, candidate_cmd_head[i]) != NULL)
		{
			typ = i;
			cout<< candidate_cmd_head[i] <<endl;
			out_cmd_head = (char *)candidate_cmd_head[i];
			for(x = 0; x< sizeof(candidate_num_head) / sizeof(candidate_num_head[0]); x++)
			{
				if(strstr(cmdbuf,candidate_num_head[x]) != NULL)
				{
					num = atoi(candidate_num_head[x]);
				}else
				{
					cout<< "num not available"<<endl;
				}

			}
			return out_cmd_head;
		}
    }

   cout<< "cmd not int candidate"<<endl;
}

static int valid_cmd_buf(const char *cmdbuf)
{
    uint32_t i;
    const char *candidate_cmd_type[] = { "kph ", "pld ", "getprop " };                                                                                                                                      
    for (i = 0; i < sizeof(candidate_cmd_type) / sizeof(candidate_cmd_type[0]); i++) {
		if (strncmp(cmdbuf, candidate_cmd_type[i], strlen(candidate_cmd_type[i])) == 0)
		{
			return true;
		}
    }

    return false;
}


int invoke_kph_api (ID_GATHER cs, int num)
{
	int ret = 0;
	switch(cs)
	{
		case KPH_GENERATE: 
			ret = kph_generate_key_legacy()
		break;
		case KPH_VERIFY_CONFIG:
			ret = kph_verify_device_config();
		break;
		case KPH_VERIFY_KEY:
			break;
		case KPH_VERIFY_DATA:
			ret = kph_verify_ta_data(6);
		break;
		default:
		printf("YQ Unkown cs =",cs);
	}

	return ret;
}

int main (int argc,char *argv[])
{
	string pm;
	int id, num = -1;

	if(argc != 0)
	{

		pm = argv[1];
		cout<<"pm = "<<pm<<endl;
		
		int i = strlen(pm.c_str());
		cout <<i<<endl;

		valid_cmd_buf(pm.c_str());
		char *obj =NULL;
		obj = parser_cmd_buf((char *)pm.c_str(),id,num);
		if(obj !=NULL)
			cout<< "parser obj char ="<<obj<<endl;

		cout<< "id ="<<id<<endl;
		ID_CANDID ucandid = (ID_CANDID)id;
		cout<<"find ID = "<<id <<" num = "<<num<<endl;

		invoke_kph_api(ucandid,num);

	}
    
	}
