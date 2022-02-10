My BTC address: 3LPCTHeKGkGtF1FfZJGskhwKiUcm7JwNAv
23 Aug 2017  19:12
Erkel  ~m~

Documentation for the 'Chaffer' system.

PLEASE NOTE: This program is not an encryption system.  
It is an obfuscation tool.  As such it may be legal in
areas where the law precludes the private citizen from
using encryption technology.  And then again it may not.
Your mileage may vary, no warranty either implied or
expressed, caveat emptor, and all other disclaimers
apply here.

Why? Well, I like the technology and nobody has done an
implementation quite like this one.  Chaffing and
Winnowing is brain dead easy to accomplish, meaning it is
fast, easy, and accurate, and when used properly provides
a high level of security in a light weight package.
That's why.

The program is set up to determine what function you wish it
to perform by the name you use to execute it.  It looks to
see what you have named it and does the approprite function.
If you have not named it one of the three names it recognizes
it won't run for you. (see INSTALLATION.txt)

The names of this program are "chaff", "winno", and "grill".
The easiest way to set it up is to make three copies of the
binary one with each name.

"chaff" is the chaffer (obfuscator).  It takes plain text input
and generates a chaffed copy on standard output.  This output
can be redirected to a file or even to winno itself.  Chaff is
aware of standard input and will happily accept data to be
chaffed from a pipe. 

"winno" is the winnower (deobfuscator).  It takes chaffed input
and removes the chaff and generates the original plain text on
standard output.  This output can be redirected to a file or
even to itself.  The usefulness of that capability is left to
the imagination of the user.  Winno is aware of standard input
and will accept data to be winnoed on that channel.

"grill" is the grill builder.  It is used to create a
printable file which can be used OFF LINE in the real world
to manually secure simple communications. It only uses the 
-k<key> and and the -d<difficulty> switchs.  The output grill
will be identified by it's key and difficulty setting so
that it can be reproduced by your friend at the other end.
Obviously you would not want to transmit that information
by the same means as the message... secret key system.

Command line switches:

-d<difficulty> : Difficulty specifies how much chaff to use.
    . It is necessary in all cases to specify the difficulty
    . because the random number generator needs to run at that
    . level when chaffing, winnowing OR building a grill.
    .
    . The default difficulty level is 2.
    .
    . A difficulty of 1 will (statistically) generate one chaff
    . character for each plain text character.  The output
    . will be about twice as many characters as the input.
    .
    . Generally the size of the output can be estimated by:
    .          size = (difficulty*input)+input
    . This is approximate and some wide variation will be
    . observed when working with small difficulties and 
    . small quantities of plain text.  Output file sizes
    . fall upon a normal curve. The algorythm does not lend
    . itself to more a more precise calculation of the exact
    . size of the output, and that is a good thing.

-k<key> : Key is any string enclosed in quotes OR any string
    . not containing spaces, newlines, or other white space.
    .
    . It is necessary in all cases to specify the key
    . because the random number generator needs to run with
    . that key when chaffing, winnowing OR building a grill.
    .
    . Keys larger than a couple of dozen characters are 
    . probably overkill.
    . 
    . Keys which produce unusually small file sizes relative
    . to their difficulty should not be used. (see above)
    .
    . Keys should not be easy to guess, as once the key is
    . known the difficulty is easily brute forced unless it
    . is very large.
    .
    . The default key is "LazyMan".  You should not use the
    . default key except for training purposes.

-i<input file> : Input file name is the name of a file which
    . you want the program to read and process.  It can be a
    . chaffed file or a winnoed file as appropriate.
    .
    . The grill program does not use the -i switch.
    .
    . Both winnow and chaff are not 'chaff aware'.  They are
    . simply working with a text tile.  It is possible to
    . chaff again a chaff file and to winnow again a winnoed
    . file.  The value of that feature is left for the user's
    . consideration.
    .
-m<"message"> : Use the message provided on the command
    . as the input to the program.  This can be data which
    . has already been chaffed for winnowing OR for another
    . layer of chaffing.  It can also be data which has
    . already been winnowed for chaffing or for further
    . winowing.  The use of this feature is also left to
    . the user's imagination.

NOTE:  If you got this far you get the prize.

The ERKEL.  You will see on the grill output the word "ERKEL".
The ERKEL specifies a row and column.  What you find at the
ERKEL is only known to you and your friend.  So I can
self-ERKEL by giving a row/column and the letter found at
that spot in the grill.  Additionally I can ask my friend
at the other end to ERKEL and he can either self-ERKEL, like
I just did or alternatively I can challenge him by giving
him a row and column and expecting him to reply with the
correct ERKEL.

Another use of the grill might be to share secret keys.
Once your friend has a copy of your grill you can always
tell him a start and end ERKEL and know that he should
understand that to be the key to be used when generating
the next grill.  For example if I sent something like
"KEY 001B 001G" then you would go to the grill and starting
witn 001B read the next secret key sequence from there to 
001G.

The additional uses of the grill are left to the imagination
of the user ;).  There are many.

Keep your difficulties, your keys and your file sizes to
yourself and ALWAYS mark off the ERKELS as you use them
and you will go far young padawan.
