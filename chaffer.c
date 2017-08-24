#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

static char *doc[] = {
"/* ============================================================= */",
"/*",
"    Programs: 'winno, chaff, grill'",
"    Initial Aug. 2017 -m- (C) All Rights Reserved",
" ",
"    This program is based upon steganography, specifically upon",
"    chaffing and winnowing as described (IIRC) by Rivest back",
"    in the late 90's.",
"",
"    When executed as 'winno' the program will take as input chaffed",
"    data, either on the command line with the -m switch or on stdin",
"    via a pipe.  It will output plain text on stdout. This can be",
"    redirected to a file.",
"",
"    When executed as 'chaff' the program will take as input plain",
"    text, either on the command line with the -m switch or on stdin",
"    via a pipe.  It will output chaffed text on stdout.  This can be",
"    redirected to a file.",
"",
"    When executed as 'grill' the program will write on stdout a",
"    grill, suitable for printing, intended for use as an off line",
"    chaffing/winnoing system. This can be redirected to a file and",
"    sent to a printer for use off line in the field."
"",
"    Switches:",
"    -k<key> : The key used to generate the chaff, which is",
"            . statistically as similar as I could get to",
"            . plain text english.",
" ",
"    -d<difficulty> : The difficulty level.  This determines the",
"            . ratio of chaff to message.  A message of one",
"            . character sent at a difficulty level of 1 will",
"            . AVERAGE 2 characters in length.  The formula is",
"            . size = difficulty*(difficulty+message_length).",
"            . Large difficulties generate large outputs.",
" ",
"    -i<input file> : The program can read chaffed messages",
"            . from files and winnow them.  It can also read",
"            . plain messages from files and chaff them.",
" ",
"    -m<\"message\"> : This switch takes a quoted string as it's",
"            . argument.  The string can be a chaffed string to be",
"            . winnoed or a plain text message to be chaffed.",
"            . Note: There are some interesting quirks related",
"            . to how the shell processes the command line which",
"            . might bite you in the ass.",
" ",
"*/",
"/* ============================================================= */",
"" /* Don't remove this null string */
};

static unsigned char letters[] = {
    'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd', 'E', 'e',
    'F', 'f', 'G', 'g', 'H', 'h', 'I', 'i', 'J', 'j',
    'K', 'k', 'L', 'l', 'M', 'm', 'N', 'n', 'O', 'o',
    'P', 'p', 'Q', 'q', 'R', 'r', 'S', 's', 'T', 't',
    'U', 'u', 'V', 'v', 'W', 'w', 'X', 'x', 'Y', 'y',
    'Z', 'z', '!', '"', '#', '$', '%', '&', '`', '(',
    ')', '*', '+', '\'','-', '.', '/', '0', '1', '2',
    '3', '4', '5', '6', '7', '8', '9', ':', ';', '<',
    '=', '>', '?', '@', '\0'
};

/*
    Based upon a 13272 char sample.
*/
static unsigned int freqs[] = {
    280, 5263,  169,  866,  229, 1960,  129, 2369,  138, 7741,
    100, 1296,   93, 1206,  123, 2955,  223, 4527,   78,   65,
     46,  460,  106, 2553,  259, 1467,  205, 4535,  105, 4729,
    144, 1255,   11,   54,  146, 4137,  304, 4186,  325, 5507,
     57, 1613,   31,  653,  107, 1015,    7,  123,   94, 1062,
      5,   66,    2,  284,    1,   51,    1,    6,  204,   53,
     53,   20,    3,  984,  252,  946,    8,  546,  460,  333,
    187,  192,  374,  153,  120,  182,  282,   54,   36,    1,
      1,    1,   12,    1,    0
};

/*
    User supplied options
*/
static unsigned int diff =0;
static unsigned int winno =0;
static unsigned int chaff =0;
static unsigned int grill =0; 
static unsigned int total_count = 0;
static unsigned char *key ="";
static char *infile ="";
static char *outfile ="";
static unsigned char *message = "";



/*
    Forward declarations of various functions.
*/
void process_command_line(int argc, char*argv[]);
void set_total_count(void);
void seed_random(char *key);
void do_usage(void);
unsigned char next_letter (void);
void winno_msg (int difficulty, unsigned char *message);
void winno_file (int difficulty, char *infile);
void chaff_msg (int difficulty, unsigned char *message);
void chaff_file (int difficulty, char *infile);
void make_grill (int difficulty, char *key);


int main (int argc, char *argv[]) {
    int i = 0;
    int c = 0;
    int tot = 0;

    /*
        Process command line
    */
    if (argc > 1) process_command_line(argc, argv);
    else {
        do_usage();
    }
    /*
        Who am I, winno, chaff or grill?
    */
        if (strstr(argv[0], "winno")!=0) {
            winno = 1;
            chaff = 0;
            grill = 0;
        }
        else if (strstr(argv[0], "chaff")!=0) {
            chaff = 1;
            winno = 0;
            grill = 0;
        }
        else if (strstr(argv[0], "grill")!=0) {
            grill = 1;
            winno = 0;
            chaff = 0;
        }
        else do_usage();
    /*
        If we got here we think we have enough to
        do a run.  Do some sanity assertions.
    */
    if (diff<=0) diff=2;
    if (strlen(key) == 0) key = "LazyMan";
    if (strlen(infile) == 0) infile = "stdin";
    outfile = "stdout";
    
    set_total_count();
    seed_random(key);

    if (grill == 1) {
        make_grill(diff, key);
        exit(EXIT_SUCCESS);
    };
    if (chaff == 1) {
            if (strlen(message)!=0) chaff_msg (diff, message);
            else chaff_file (diff, infile);
            exit(EXIT_SUCCESS);
    };  
    if (winno == 1) {
            if (strlen(message)!=0) winno_msg (diff, message);
            else winno_file (diff, infile);
            exit(EXIT_SUCCESS);
    };

/* DEBUG
    for (i=0; i<26; i++) {
        printf("Letter: %c = %i\n", letters[i], freqs[i]);
        tot += freqs[i];
    }
    printf("Total %i\n", tot);
*/
/* DEBUG
   make_grill(2); 
*/  exit(0);
}

/*
    Set total count of characters in our database
*/
void set_total_count(void) {
    int ix = 0;
    while (freqs[ix]!=0) {
        total_count += freqs[ix];
        ++ix;
    }
    return;
}
/*
    Seed the random number generator with a deterministic value.
*/
void seed_random(char *key) {
/*
    We need something we can duplicate
    every time here.  So ....
*/
        int i = 0;
        int l = strlen(key);
        unsigned int seed = 0;

        seed = (unsigned int) *key;
/* DEBUG
        printf("%u\n", (unsigned int)*key);
*/      for (i=0; i<l; ++i) {
            seed ^= key[i];
            seed *= key[i];
        }
/* DEBUG
        printf("%u <= Key\n", seed);
*/  srand(seed);
    return;
}

/*
    Get the next character of chaff from the stream.
*/
unsigned char next_letter (void) {
    static unsigned char last = ' ';
    unsigned char pick =' ';
    int ix = 0;
    int cnt = 0;
    /* 
    The magic number below is the total
    number of chars in the sample space.
    */
    int num = rand() % total_count;
/* DEBUG
    printf(" %u=", num);
*/  while (cnt < num) {
        cnt += freqs[ix];
        ++ix;
    }
    --ix;
    /*
        Don't allow doubles -- This will likely
        skew the probabilities a little but we
        are not trying to keep NSA at bay.
    */
    pick = letters[ix];
    if (pick==last) {
        pick = next_letter();
    }
    last = pick;
    return pick;
}

/*
    Take message from command line (-m switch) and winno it.
*/
void winno_msg (int difficulty, unsigned char *message) {
    FILE *output = stdout;
    unsigned char next_msg_char =0;
    unsigned char next_chaff =0;
    int ix = 0;
  
    next_msg_char = message[ix];
    while (next_msg_char !=0) {
        if ((rand() % (difficulty+1)) != 0) {
            /* Sync the stream. */
            next_chaff = next_letter();
            ++ix;
        }
        else {
            fwrite(&next_msg_char, sizeof(next_msg_char), 1, output);
            ++ix;
            next_msg_char = message[ix];
        }
    }
    fsync(fileno(output));
/*
    Had a very hard time getting the data to flush before the
    program terminated.  Finally added this call to system.
*/
    system("sync; sync;");
    return;
}
/*
    Clean the chaff from an input file or stdin.
*/
void winno_file (int difficulty, char *infile) {
    FILE *output = stdout;
    FILE *input;
    int closein = 0;
    unsigned char next_msg_char =0;
    unsigned char next_chaff =0;
  
    if (strcmp(infile, "stdin")!=0) {
        input = fopen(infile, "r");
        closein = 1;
    }
    else input = stdin;

    while (!feof(input) && !ferror(input)) {
        if ((rand() % (difficulty+1)) != 0) {
            /* Have to keep the stream synced or above rand
               fails to generate the correct number.
            */
            next_chaff = next_letter();
            /*
                Have to read to get the chaff out of the file
            */
            fread(&next_msg_char, sizeof(next_msg_char), 1, input);
            /* next_chaff should equal next_msg_char here */
        }
        else {
            fread(&next_msg_char, sizeof(next_msg_char), 1, input);
            fwrite(&next_msg_char, sizeof(next_msg_char), 1, output);
        }
    }
    if (closein) fclose(input);
    fsync(fileno(output));
/*
    Had a very hard time getting the data to flush before the
    program terminated.  Finally added this call to system.
*/
    system("sync; sync;");
    return;
}

/*
    Take message from command line (-m switch) and chaff it.
*/
void chaff_msg (int difficulty, unsigned char *message) {
    FILE *output = stdout;
    unsigned char next_msg_char =0;
    unsigned char next_chaff =0;
    int ix = 0;
    
    next_msg_char = message[ix];
    while (next_msg_char !=0) {
        if ((rand() % (difficulty+1)) != 0) {
            next_chaff = next_letter();
            fwrite(&next_chaff, 
                sizeof(next_chaff), 1, output);
        }
        else {
            fwrite(&next_msg_char, 
                sizeof(next_msg_char), 1, output);
            ++ix;
            next_msg_char = message[ix];
        }
    }
    fsync(fileno(output));
/*
    Had a very hard time getting the data to flush before the
    program terminated.  Finally added this call to system.
*/
    system("sync; sync;");
    return;
}

/*
    Chaff an input file or stdin.
*/
void chaff_file (int difficulty, char *infile) {
    FILE *output = stdout;
    FILE *input;
    int closein = 0;
    unsigned char next_msg_char =0;
    unsigned char next_char =0;
    unsigned char next_chaff =0;

    if (strcmp(infile, "stdin")!=0) {
        input = fopen(infile, "r");
        closein = 1;
    }
    else input = stdin;

    fread(&next_msg_char, sizeof(next_msg_char), 1, input);
    while (!feof(input) && !ferror(input)) {
        if ((rand() % (difficulty+1)) != 0) {
            next_chaff = next_letter();
            fwrite(&next_chaff, sizeof(next_chaff), 1, output);
        }
        else {
            fwrite(&next_msg_char, sizeof(next_msg_char), 1, output);
            fread(&next_msg_char, sizeof(next_msg_char), 1, input);
        }
    }
 
    if (closein) fclose(input);
    fsync(fileno(output));
/*
    Had a very hard time getting file buffers to flush before
    program terminated.  Finally added this call to system.
*/
    system("sync; sync;");
    return;
}
/*
    Create the grill
*/
void make_grill (int difficulty, char *key) {
static char *sep = "--------------------------------------------------------------------------------";
static char *ppp = "================================================================================";
static char top[] =
    "|ERKEL|| A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R |";
    FILE *output = stdout;
    unsigned int line = 0; 
    int column = 0;

    /*
        We will identify the page with the key and
        the difficulty.  This should be sufficient for
        the far end to generate an identical grill.
    */
    fprintf(output, "\n                        Chaff Key: %s, Difficulty: %u\n\n", 
            key, difficulty);
    fprintf(output,"%s\n", ppp);
    fprintf(output, "%s\n", top);
    fprintf(output,"%s\n", ppp);
    for (line = 0; line < 30; ++line) {        
        fprintf(output, "| %03u |", line);
        for (column = 7; column < 79; column += 4){
            if (rand() % (difficulty+1)!=0) {
                fprintf(output, "| %c ", next_letter());
            }
            else {
                fprintf(output, "%s", "|   ");
            }
        }
        fprintf(output, "|\n");
        fprintf(output, "%s\n", sep);
    }
    fsync(fileno(output));
    /*
        Had a very hard time getting file buffers to flush before
        program terminated.  Finally added this call to system.
    */
    system("sync; sync;");
    return;
}

/*
    Process the command line here and store the information we
    need in some global variables declared at top of file.
*/
void process_command_line(int argc, char *argv[]) {
    int c = 0;

    /* Declared in unixstd.h */
    opterr = 0;
    
    while ((c = getopt (argc, argv, "m:k:i:d:")) != -1)
    switch (c) {
        case 'm': /* Get Message */
                message = optarg;
                if (message[0]=='-') {
                    do_usage();
                }
        break;
        case 'k': /* Get Key */
                key = optarg;
                if (key[0]=='-') {
                    do_usage();
                }
        break;
        case 'i': /* Get infile */
                infile = optarg;
                if (infile[0]=='-') {
                    do_usage();
                }
        break;
        case 'd': /* Get difficulty */
                diff = atoi(optarg);
                if (diff<=0) {
                    do_usage();
                }
        break;
        default:
                do_usage();
        break;
    }
/* DEBUG
    printf("SWITCH IS: %c\n", c);
*/
    return;
}

/*
    Do usage
*/
void do_usage(void) {
    int ix = 0;
    while (doc[ix]!=0) {
        printf("%s\n", doc[ix]);
        ++ix;
    }
    exit(EXIT_FAILURE);
    return;
}