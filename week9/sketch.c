#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "display.h"
#include <string.h> // TODO: check if this is OK

// TODO: upgrade the run function, adding functions to support it.

// constants representing possible opcodes (the first two bits of an instruction byte)
const int DX = 0;
const int DY = 1;
const int DT = 2;
const int PEN = 3;

// storing bytes as unsigned chars
typedef unsigned char Byte;

// TODO: try fix enumerated type representing an opcode
// typedef enum {DX, DY, DT, PEN} OpCode;

// .sketch file instruction byte
typedef struct InstructionByte InstructionByte;
struct InstructionByte {
  Byte raw; // store a copy of the uninterpreted raw instruction
  // OpCode opcode; // instruction opcode (first 2 bits)
  int opcode; // instruction opcode (first 2 bits) TODO: change to enumerated type
  signed int operand; // instruction operand (last 6 bits)
};

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

int printBitsNL (Byte wtf){
  printBits(1, &wtf);
  // printf("\n");
  return 0;
}

int testWTF(Byte wtf){
  printBitsNL(wtf);
  wtf = wtf<<2;
  printBitsNL(wtf);
  wtf = ~wtf;
  printBitsNL(wtf);
  wtf += 1;
  printBitsNL(wtf);
  wtf = wtf>>2;
  printf("%d\n", wtf);
  printBitsNL(wtf);
  return 0;
}

_Bool operandIsPositive (Byte wtf){
  // printf("is operand of %d positive or negative?\nbinary: ", wtf);
  // printBitsNL(wtf);
  // printf("shift2L\nbinary: ");
  wtf = wtf<<2;
  // printBitsNL(wtf);
  // printf("shift2R\nbinary: ");
  wtf = wtf>>2;
  // printBitsNL(wtf);
  // printf("result: %i\n", wtf);
  if (wtf>>2 & 1) {
    return true;
  } else {
    return false;
  }
}

char positiveOrNegative (Byte x){
  return (operandIsPositive(x) ? 'p' : 'n');
}

signed int extractOperand (Byte input){
  _Bool needsFlipping = !operandIsPositive(input);
  if (needsFlipping) {
    Byte masked = input & 0x3f;
    return ~masked + 1;
  } else {
    return input & 0x3f;
  }
}

int extractOpcode (Byte input){
  Byte masked = input & 0xC0;
  return masked >> 6;
}

/**
 * read bytes from (already opened) in file, unpack each byte into an opcode and operand, and use those to make suitable calls to display functions
 * @param  *in file to read
 * @param  d  display to call functions to
 * @return    [description]
 */
int interpret(FILE *in, display *d) {
  /* code */
  return 0;
}

void printPosNeg (Byte testing){
  printf("0x%02hhx is %c\n", testing, positiveOrNegative(testing));
}

// char *stringifyOpcode (int code, char *destination) {
char *stringifyOpcode (int code) {
  char *result;

  switch (code) {
    case 0:
      result = "DX";
    case 1:
      result = "DY";
    case 2:
      result = "DT";
    case 3:
      result = "PEN";
    default:
      result = "ERROR";
  }

  // strcpy(destination, result);
  return result;
}

// Read sketch instructions from the given file.  If test is NULL, display the
// result in a graphics window, else check the graphics calls made.
void run(char *filename, char *test[]) {
    Byte example = 0x7D;

    InstructionByte exampleInstruction = {
      .raw = example,
      .opcode = extractOpcode(example),
      .operand = extractOperand(example)
    };

    char parsedOpcode[4];
    strcpy(parsedOpcode, stringifyOpcode(exampleInstruction.opcode));

    printf("the example instruction (0x%02hhx) translates to %s %i.\n", example, parsedOpcode, exampleInstruction.operand);
    printf("binary:");
    printBitsNL(exampleInstruction.raw);

    // printf("lol examples of signed ints: [%i,%i]", minusOne, minusNine);
    // printf("")

    // FILE *in = fopen(filename, "rb");
    // if (in == NULL) {
    //     fprintf(stderr, "Can't open %s\n", filename);
    //     exit(1);
    // }
    // display *d = newDisplay(filename, 1024, 1024, test);
    // end(d);
    // fclose(in);
}

// ----------------------------------------------------------------------------
// Nothing below this point needs to be changed.
// ----------------------------------------------------------------------------

// Forward declaration of test data.
char **lineTest, **squareTest, **boxTest, **oxoTest, **diagTest, **crossTest,
     **clearTest, **keyTest, **pausesTest, **fieldTest, **lawnTest;

void testSketches() {
    // Stage 1
    run("line.sketch", lineTest);
    run("square.sketch", squareTest);
    run("box.sketch", boxTest);
    run("oxo.sketch", oxoTest);

    // Stage 2
    run("diag.sketch", diagTest);
    run("cross.sketch", crossTest);

    // Stage 3
    run("clear.sketch", clearTest);
    run("key.sketch", keyTest);

    // Stage 4
    run("pauses.sketch", pausesTest);
    run("field.sketch", fieldTest);
    run("lawn.sketch", lawnTest);
}

int main(int n, char *args[n]) {
    if (n == 1) testSketches();
    else if (n == 2) run(args[1], NULL);
    else {
        fprintf(stderr, "Usage: sketch [file.sketch]");
        exit(1);
    }
}

// Each test is a series of calls, stored in a variable-length array of strings,
// with a NULL terminator.

// The calls that should be made for line.sketch.
char **lineTest = (char *[]) {
    "line(d,30,30,60,30)", NULL
};

// The calls that should be made for square.sketch.
char **squareTest = (char *[]) {
    "line(d,30,30,60,30)", "line(d,60,30,60,60)",
    "line(d,60,60,30,60)","line(d,30,60,30,30)", NULL
};

// The calls that should be made for box.sketch.
char **boxTest = (char *[]) {
    "line(d,30,30,32,30)", "pause(d,10)", "line(d,32,30,34,30)", "pause(d,10)",
    "line(d,34,30,36,30)", "pause(d,10)", "line(d,36,30,38,30)", "pause(d,10)",
    "line(d,38,30,40,30)", "pause(d,10)", "line(d,40,30,42,30)", "pause(d,10)",
    "line(d,42,30,44,30)", "pause(d,10)", "line(d,44,30,46,30)", "pause(d,10)",
    "line(d,46,30,48,30)", "pause(d,10)", "line(d,48,30,50,30)", "pause(d,10)",
    "line(d,50,30,52,30)", "pause(d,10)", "line(d,52,30,54,30)", "pause(d,10)",
    "line(d,54,30,56,30)", "pause(d,10)", "line(d,56,30,58,30)", "pause(d,10)",
    "line(d,58,30,60,30)", "pause(d,10)", "line(d,60,30,60,32)", "pause(d,10)",
    "line(d,60,32,60,34)", "pause(d,10)", "line(d,60,34,60,36)", "pause(d,10)",
    "line(d,60,36,60,38)", "pause(d,10)", "line(d,60,38,60,40)", "pause(d,10)",
    "line(d,60,40,60,42)", "pause(d,10)", "line(d,60,42,60,44)", "pause(d,10)",
    "line(d,60,44,60,46)", "pause(d,10)", "line(d,60,46,60,48)", "pause(d,10)",
    "line(d,60,48,60,50)", "pause(d,10)", "line(d,60,50,60,52)", "pause(d,10)",
    "line(d,60,52,60,54)", "pause(d,10)", "line(d,60,54,60,56)", "pause(d,10)",
    "line(d,60,56,60,58)", "pause(d,10)", "line(d,60,58,60,60)", "pause(d,10)",
    "line(d,60,60,58,60)", "pause(d,10)", "line(d,58,60,56,60)", "pause(d,10)",
    "line(d,56,60,54,60)", "pause(d,10)", "line(d,54,60,52,60)", "pause(d,10)",
    "line(d,52,60,50,60)", "pause(d,10)", "line(d,50,60,48,60)", "pause(d,10)",
    "line(d,48,60,46,60)", "pause(d,10)", "line(d,46,60,44,60)", "pause(d,10)",
    "line(d,44,60,42,60)", "pause(d,10)", "line(d,42,60,40,60)", "pause(d,10)",
    "line(d,40,60,38,60)", "pause(d,10)", "line(d,38,60,36,60)", "pause(d,10)",
    "line(d,36,60,34,60)", "pause(d,10)", "line(d,34,60,32,60)", "pause(d,10)",
    "line(d,32,60,30,60)", "pause(d,10)", "line(d,30,60,30,58)", "pause(d,10)",
    "line(d,30,58,30,56)", "pause(d,10)", "line(d,30,56,30,54)", "pause(d,10)",
    "line(d,30,54,30,52)", "pause(d,10)", "line(d,30,52,30,50)", "pause(d,10)",
    "line(d,30,50,30,48)", "pause(d,10)", "line(d,30,48,30,46)", "pause(d,10)",
    "line(d,30,46,30,44)", "pause(d,10)", "line(d,30,44,30,42)", "pause(d,10)",
    "line(d,30,42,30,40)", "pause(d,10)", "line(d,30,40,30,38)", "pause(d,10)",
    "line(d,30,38,30,36)", "pause(d,10)", "line(d,30,36,30,34)", "pause(d,10)",
    "line(d,30,34,30,32)", "pause(d,10)", "line(d,30,32,30,30)", "pause(d,10)",
    NULL
};

// The calls that should be made for box.sketch.
char **oxoTest = (char *[]) {
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,30,40,60,40)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,30,50,60,50)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,40,30,40,60)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,50,30,50,60)", NULL
};

// The calls that should be made for diag.sketch.
char **diagTest = (char *[]) {
    "line(d,30,30,60,60)", NULL
};

// The calls that should be made for cross.sketch.
char **crossTest = (char *[]) {
    "line(d,30,30,60,60)", "line(d,60,30,30,60)", NULL
};

// The calls that should be made for clear.sketch.
char **clearTest = (char *[]) {
    "line(d,30,40,60,40)", "line(d,30,50,60,50)", "line(d,40,30,40,60)",
    "line(d,50,30,50,60)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "clear(d)", "line(d,30,30,60,60)",
    "line(d,60,30,30,60)", NULL
};

// The calls that should be made for key.sketch.
char **keyTest = (char *[]) {
    "line(d,30,40,60,40)", "line(d,30,50,60,50)", "line(d,40,30,40,60)",
    "line(d,50,30,50,60)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "key(d)", "clear(d)", "line(d,30,30,60,60)",
    "line(d,60,30,30,60)", NULL
};

// The calls that should be made for diag.sketch.
char **pausesTest = (char *[]) {
    "pause(d,0)", "pause(d,0)", "pause(d,127)", "pause(d,128)", "pause(d,300)",
    "pause(d,0)", "pause(d,71469)", NULL
};

// The calls that should be made for field.sketch.
char **fieldTest = (char *[]) {
    "line(d,30,30,170,30)", "line(d,170,30,170,170)",
    "line(d,170,170,30,170)", "line(d,30,170,30,30)", NULL
};

// The calls that should be made for field.sketch.
char **lawnTest = (char *[]) {
    "cline(d,30,30,170,30,16711935)", "cline(d,170,30,170,170,16711935)",
    "cline(d,170,170,30,170,16711935)", "cline(d,30,170,30,30,16711935)",
    NULL
};
