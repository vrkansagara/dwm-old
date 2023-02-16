//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
    /*Icon*/    /*Command*/     /*Update Interval*/ /*Update Signal*/
    {"Mem:", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g", 30,     0},
    {"IST:", "TZ=':Asia/Kolkata' date +'%a %d %b[%r]'",                 1,      0},
    {"CET:", "TZ=':Europe/Amsterdam' date +'%a %d %b[%r]'",             1,      0},
     {""," brightness up 10000",1,0},
     {""," brightness down 10000",1,0},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = " | ";
static unsigned int delimLen = 5;
