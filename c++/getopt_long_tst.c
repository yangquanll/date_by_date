#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
static struct option long_options[] =
{
    { "help",   no_argument,       0, 'h' },
    { "cmd",    required_argument, 0, 'c' },
    { "name",   required_argument, 0, 'n' },
    { "input",  required_argument, 0, 'i' },
    { "output", required_argument, 0, 'o' },
    { "chipid", required_argument, 0, 'x' },
    { "serialno", required_argument, 0, 's' },
    { "state",  required_argument, 0, 't' },
    { "tag", required_argument, 0, 'g' },
    { "perm", required_argument, 0, 'p' },
    {0, 0, 0, 0}
};
int main(int argc, const char* argv[])
{
   int ret = 0;
    int c;
    while (1) {
        if (ret != 0) {
            break;
        }
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, (char* const *)argv, "hcn:i:o:s:x:w:t:g:p:",
                long_options, &option_index);

        /* Detect the end of the options or some other error */
        if (c == -1) {
            break;
        }

        switch (c) {
            case 0:
                /* meaningful when this option set a flag, we did not use this feature. */
                break;
            case 'h':
//              g_print_help = 1;
  				printf("cmd =  h \n");
				break;
            case 'c':
//                g_cmd_arg = optarg;
                 printf("cmd =  c \n"); 
		break;
            case 'n':
 //               g_name_arg = optarg;
 		printf("cmd =  n \n");
                break;
            case 'i':
		printf("cmd =  i \n");
 //               g_input_arg = optarg;
                break;
            case 'o':
 //               g_output_arg = optarg;
                break;
            case 's':
//                g_sn_arg = optarg;
                break;
            case 'x':
 //               g_chipid_arg = optarg;
                break;
            case 'w':
  //              g_flag_arg = optarg;
                break;
            case 't':
 //               g_state_arg = optarg;
                break;
            case 'g':
//                g_tag_arg = optarg;
                break;
            case 'p':
//               g_perm_arg = optarg;
                break;
            default:
//                g_print_help = 1;
                ret = -1;
        }
    }

    return ret;
}

