/*
    Tanzir Ahmed
    Department of Computer Science & Engineering
    Texas A&M University
    Date  : 2/8/19
 */
#include "common.h"
#include "FIFOreqchannel.h"

using namespace std;


int main(int argc, char *argv[]){
    // start up the server from the client-side
    if(fork() == 0) {
        char *argv[] = {"./dataserver", NULL};
        execvp(argv[0], argv);
    }
    
    srand(time_t(NULL));
    
    bool file_requested = false;
    bool new_channel = false;
    int patient_requested;
    double time_requested;
    int ecg_num_requested;
    string filename;
    
    int option;
    int errflag = 0;
    extern char *optarg;
    extern int optind, optopt;

    while ((option = getopt(argc, argv, "p:t:e:f:c")) != -1) {
        switch(option) {
            case 'p':
                if(isdigit(optarg[0]))
                    patient_requested = atoi(optarg);
                else
                    errflag++;
                break;
            case 't':
                if(isdigit(optarg[0]))
                    time_requested = atof(optarg);
                else
                    errflag++;
                break;
            case 'e':
                if(isdigit(optarg[0]))
                    ecg_num_requested = atoi(optarg);
                else
                    errflag++;
                break;
            case 'f':
                filename = optarg;
                file_requested = true;
                if(new_channel)
                    errflag++;
                break;
            case 'c':
                new_channel = true;
                if(file_requested)
                    errflag++;
                break;
            case '?':
                errflag++;
        }
    }
    if (errflag) {
        fprintf(stderr, "usage: client [-p <person>] [-t <time>] [-e <ecg number>] OR usage: client [-f <file>]\n");
        return 1;
    }
    
    // cout << patient_requested << endl << time_requested << endl << ecg_num_requested << endl;

    FIFORequestChannel chan ("control", FIFORequestChannel::CLIENT_SIDE);
    vector<FIFORequestChannel*> more_channels; // user may create and join a channel

    if(file_requested) {
        // will reuse this block of memory
        const char* filename_cstr = filename.c_str();
        int block_size = sizeof(filemsg) + sizeof(filename_cstr);
        char* block = new char[block_size];
        filemsg* getFileLength = (filemsg*) block; // set pointers
        char* filename_to_server = block + sizeof(filemsg); // this pointer's location is constant
        *getFileLength = filemsg(0, 0); // write the data
        strcpy(filename_to_server, filename_cstr); // won't need to ever change this
        
        chan.cwrite(getFileLength, block_size);
        char* buf = chan.cread();
        __int64_t* len = (__int64_t*) buf; // cast char ptr to int ptr
//        cout << "len = " << *len << endl;

        __int64_t len_remaining = *len;
        
        // open a file in received/ directory
        string new_file = "./received/" + filename;
        cout << new_file << endl;
        int fd;
        if((fd = open(new_file.c_str(), O_WRONLY|O_CREAT)) < 0) {
            perror("open");
            _exit(1);
        }
        cout << "fd: " << fd << endl;
        filemsg* msg = (filemsg*) block; // re-use same block of memory
        while(len_remaining > 0) { // assumes no remainder
            *msg = filemsg(*len - len_remaining, MAX_MESSAGE % len_remaining); // write the data
            chan.cwrite(msg, block_size);
            buf = chan.cread();
            // write to a file I've opened
            if (write(fd, buf, MAX_MESSAGE) < 0) {
                perror("write");
                _exit(1);
            }
            len_remaining -= MAX_MESSAGE;
        }

        if(close(fd) < 0) {
            perror("close");
            exit(1);
        }
        
        delete[] block;
    } else if(new_channel) {
        MESSAGE_TYPE m = NEWCHANNEL_MSG;
        chan.cwrite(&m, sizeof (m)); // just send the message
        char* buf = chan.cread(); // contains name of new channel
        string name = buf;
        FIFORequestChannel* new_chan = new FIFORequestChannel(name, FIFORequestChannel::CLIENT_SIDE);
        more_channels.push_back(new_chan);
    } else {
        datamsg d = datamsg(patient_requested, time_requested, ecg_num_requested);
        chan.cwrite(&d, sizeof (d));
        char* buf = chan.cread();
        double* reply = (double*) buf;
        cout << *reply << endl;
    }

    // closing the channel    
    MESSAGE_TYPE m = QUIT_MSG;
    chan.cwrite(&m, sizeof (MESSAGE_TYPE));
    for(int i = 0; i < more_channels.size(); i++)
        more_channels[i]->cwrite(&m, sizeof (MESSAGE_TYPE));
    for(int i = 0; i < more_channels.size(); i++)
        delete more_channels[i];
    wait(0); // wait for server process to quit
}
