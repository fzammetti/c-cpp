
#include "stdafx.h"
#include <iostream.h>

int LEFT;
int RIGHT;

// This global array is used to perform function PC-2 on the subkeys, by relocating 4 bits at a time
int pc2bytes[14][64] = {{0,0x4,0x20000000,0x20000004,0x10000,0x10004,0x20010000,0x20010004,0x200,0x204,0x20000200,0x20000204,0x10200,0x10204,0x20010200,0x20010204},
												{0,0x1,0x100000,0x100001,0x4000000,0x4000001,0x4100000,0x4100001,0x100,0x101,0x100100,0x100101,0x4000100,0x4000101,0x4100100,0x4100101},
												{0,0x8,0x800,0x808,0x1000000,0x1000008,0x1000800,0x1000808,0,0x8,0x800,0x808,0x1000000,0x1000008,0x1000800,0x1000808},
												{0,0x200000,0x8000000,0x8200000,0x2000,0x202000,0x8002000,0x8202000,0x20000,0x220000,0x8020000,0x8220000,0x22000,0x222000,0x8022000,0x8222000},
												{0,0x40000,0x10,0x40010,0,0x40000,0x10,0x40010,0x1000,0x41000,0x1010,0x41010,0x1000,0x41000,0x1010,0x41010},
												{0,0x400,0x20,0x420,0,0x400,0x20,0x420,0x2000000,0x2000400,0x2000020,0x2000420,0x2000000,0x2000400,0x2000020,0x2000420},
												{0,0x10000000,0x80000,0x10080000,0x2,0x10000002,0x80002,0x10080002,0,0x10000000,0x80000,0x10080000,0x2,0x10000002,0x80002,0x10080002},
												{0,0x10000,0x800,0x10800,0x20000000,0x20010000,0x20000800,0x20010800,0x20000,0x30000,0x20800,0x30800,0x20020000,0x20030000,0x20020800,0x20030800},
												{0,0x40000,0,0x40000,0x2,0x40002,0x2,0x40002,0x2000000,0x2040000,0x2000000,0x2040000,0x2000002,0x2040002,0x2000002,0x2040002},
												{0,0x10000000,0x8,0x10000008,0,0x10000000,0x8,0x10000008,0x400,0x10000400,0x408,0x10000408,0x400,0x10000400,0x408,0x10000408},
												{0,0x20,0,0x20,0x100000,0x100020,0x100000,0x100020,0x2000,0x2020,0x2000,0x2020,0x102000,0x102020,0x102000,0x102020},
												{0,0x1000000,0x200,0x1000200,0x200000,0x1200000,0x200200,0x1200200,0x4000000,0x5000000,0x4000200,0x5000200,0x4200000,0x5200000,0x4200200,0x5200200},
												{0,0x1000,0x8000000,0x8001000,0x80000,0x81000,0x8080000,0x8081000,0x10,0x1010,0x8000010,0x8001010,0x80010,0x81010,0x8080010,0x8081010},
												{0,0x4,0x100,0x104,0,0x4,0x100,0x104,0x1,0x5,0x101,0x105,0x1,0x5,0x101,0x105}};

// These global arrays are the S selection functions used to encrypt or decrypt
int spfunction[9][64]  = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
													{0x808200,0,0x8000,0x808202,0x808002,0x8202,0x2,0x8000,0x200,0x808200,0x808202,0x200,0x800202,0x808002,0x800000,0x2,0x202,0x800200,0x800200,0x8200,0x8200,0x808000,0x808000,0x800202,0x8002,0x800002,0x800002,0x8002,0,0x202,0x8202,0x800000,0x8000,0x808202,0x2,0x808000,0x808200,0x800000,0x800000,0x200,0x808002,0x8000,0x8200,0x800002,0x200,0x2,0x800202,0x8202,0x808202,0x8002,0x808000,0x800202,0x800002,0x202,0x8202,0x808200,0x202,0x800200,0x800200,0,0x8002,0x8200,0,0x808002},
													{0x40084010,0x40004000,0x4000,0x84010,0x80000,0x10,0x40080010,0x40004010,0x40000010,0x40084010,0x40084000,0x40000000,0x40004000,0x80000,0x10,0x40080010,0x84000,0x80010,0x40004010,0,0x40000000,0x4000,0x84010,0x40080000,0x80010,0x40000010,0,0x84000,0x4010,0x40084000,0x40080000,0x4010,0,0x84010,0x40080010,0x80000,0x40004010,0x40080000,0x40084000,0x4000,0x40080000,0x40004000,0x10,0x40084010,0x84010,0x10,0x4000,0x40000000,0x4010,0x40084000,0x80000,0x40000010,0x80010,0x40004010,0x40000010,0x80010,0x84000,0,0x40004000,0x4010,0x40000000,0x40080010,0x40084010,0x84000},
													{0x104,0x4010100,0,0x4010004,0x4000100,0,0x10104,0x4000100,0x10004,0x4000004,0x4000004,0x10000,0x4010104,0x10004,0x4010000,0x104,0x4000000,0x4,0x4010100,0x100,0x10100,0x4010000,0x4010004,0x10104,0x4000104,0x10100,0x10000,0x4000104,0x4,0x4010104,0x100,0x4000000,0x4010100,0x4000000,0x10004,0x104,0x10000,0x4010100,0x4000100,0,0x100,0x10004,0x4010104,0x4000100,0x4000004,0x100,0,0x4010004,0x4000104,0x10000,0x4000000,0x4010104,0x4,0x10104,0x10100,0x4000004,0x4010000,0x4000104,0x104,0x4010000,0x10104,0x4,0x4010004,0x10100},
													{0x80401000,0x80001040,0x80001040,0x40,0x401040,0x80400040,0x80400000,0x80001000,0,0x401000,0x401000,0x80401040,0x80000040,0,0x400040,0x80400000,0x80000000,0x1000,0x400000,0x80401000,0x40,0x400000,0x80001000,0x1040,0x80400040,0x80000000,0x1040,0x400040,0x1000,0x401040,0x80401040,0x80000040,0x400040,0x80400000,0x401000,0x80401040,0x80000040,0,0,0x401000,0x1040,0x400040,0x80400040,0x80000000,0x80401000,0x80001040,0x80001040,0x40,0x80401040,0x80000040,0x80000000,0x1000,0x80400000,0x80001000,0x401040,0x80400040,0x80001000,0x1040,0x400000,0x80401000,0x40,0x400000,0x1000,0x401040},
													{0x80,0x1040080,0x1040000,0x21000080,0x40000,0x80,0x20000000,0x1040000,0x20040080,0x40000,0x1000080,0x20040080,0x21000080,0x21040000,0x40080,0x20000000,0x1000000,0x20040000,0x20040000,0,0x20000080,0x21040080,0x21040080,0x1000080,0x21040000,0x20000080,0,0x21000000,0x1040080,0x1000000,0x21000000,0x40080,0x40000,0x21000080,0x80,0x1000000,0x20000000,0x1040000,0x21000080,0x20040080,0x1000080,0x20000000,0x21040000,0x1040080,0x20040080,0x80,0x1000000,0x21040000,0x21040080,0x40080,0x21000000,0x21040080,0x1040000,0,0x20040000,0x21000000,0x40080,0x1000080,0x20000080,0x40000,0,0x20040000,0x1040080,0x20000080},
													{0x10000008,0x10200000,0x2000,0x10202008,0x10200000,0x8,0x10202008,0x200000,0x10002000,0x202008,0x200000,0x10000008,0x200008,0x10002000,0x10000000,0x2008,0,0x200008,0x10002008,0x2000,0x202000,0x10002008,0x8,0x10200008,0x10200008,0,0x202008,0x10202000,0x2008,0x202000,0x10202000,0x10000000,0x10002000,0x8,0x10200008,0x202000,0x10202008,0x200000,0x2008,0x10000008,0x200000,0x10002000,0x10000000,0x2008,0x10000008,0x10202008,0x202000,0x10200000,0x202008,0x10202000,0,0x10200008,0x8,0x2000,0x10200000,0x202008,0x2000,0x200008,0x10002008,0,0x10202000,0x10000000,0x200008,0x10002008},
													{0x100000,0x2100001,0x2000401,0,0x400,0x2000401,0x100401,0x2100400,0x2100401,0x100000,0,0x2000001,0x1,0x2000000,0x2100001,0x401,0x2000400,0x100401,0x100001,0x2000400,0x2000001,0x2100000,0x2100400,0x100001,0x2100000,0x400,0x401,0x2100401,0x100400,0x1,0x2000000,0x100400,0x2000000,0x100400,0x100000,0x2000401,0x2000401,0x2100001,0x2100001,0x1,0x100001,0x2000000,0x2000400,0x100000,0x2100400,0x401,0x100401,0x2100400,0x401,0x2000001,0x2100401,0x2100000,0x100400,0,0x1,0x2100401,0,0x100401,0x2100000,0x400,0x2000001,0x2000400,0x400,0x100001},
													{0x8000820,0x800,0x20000,0x8020820,0x8000000,0x8000820,0x20,0x8000000,0x20020,0x8020000,0x8020820,0x20800,0x8020800,0x20820,0x800,0x20,0x8020000,0x8000020,0x8000800,0x820,0x20800,0x20020,0x8020020,0x8020800,0x820,0,0,0x8020020,0x8000020,0x8000800,0x20820,0x20000,0x20820,0x20000,0x8020800,0x800,0x20,0x8020020,0x800,0x20820,0x8000800,0x20,0x8000020,0x8020000,0x8020020,0x8000000,0x20000,0x8000820,0,0x8020820,0x20020,0x8000020,0x8020000,0x8000800,0x8000820,0,0x8020820,0x20800,0x20800,0x820,0x820,0x20020,0x8000000,0x8020800}};


void main() {
	cout << "Triple-DES w/outer-CBC TEST\nFrank W. Zammetti\n\n";
	cout << "Using the following keys:\n-------------------------\n";
	cout << "Key1: 'CheckFre' (43-68-65-63-6B-46-72-65)\n";
	cout << "Key2: 'eCorpora' (65-43-6F-72-70-6F-72-61)\n";
	cout << "Key3: 'tion-WBP' (74-69-6F-6E-2D-57-42-50)\n\n";
	cout << "Using the following initialization vector:\n------------------------------------------\n";
	cout << "'Trip-DES' (54-72-69-70-2D-44-45-53)\n\n";
	cout << "Encrypting the following test message:\n--------------------------------------\n";
	cout << "'the way money moves     '\n\n";
	cout << "Result of encryption:\n--------------------\n";
	cout << "xxxxxxxxxxxxxxxxxxxxxxxx\n(xx-xx-xx-xx-xx-xx-xx-xx / xx-xx-xx-xx-xx-xx-xx-xx / xx-xx-xx-xx-xx-xx-xx-xx)\n";
	cout << "(xx-xx-xx-xx-xx-xx-xx-xx / xx-xx-xx-xx-xx-xx-xx-xx / xx-xx-xx-xx-xx-xx-xx-xx)\n\n";



// Do3DES - This performs triple-DES, an encryption, decryption and then encryption (EDE)
// Paramters: key1 ...... First key
//            key2 ...... Second key
//            key3 ...... Third key
//            ed_func ... Encrypt (1) or decrypt (0)
//            message ... String to encrypt or decrypt
//            iv ........ Initialization vector
function Do3DES(key1, key2, key3, message, ed_func, iv) {
  key = key1 + key2 + key3;
  var keyarray     = stringToIntegers(key), resultarray;
  var messagearray = stringToIntegers(message);
  var ivarray      = iv ? stringToIntegers(iv) : new Array();
  resultarray      = des_crypt(keyarray[ed_func ? 0:4],  keyarray[ed_func ? 1:5], messagearray, ed_func, ivarray);
  resultarray      = des_crypt(keyarray[2], keyarray[3], resultarray,             !ed_func,     ivarray);
  resultarray      = des_crypt(keyarray[ed_func ? 4:0],  keyarray[ed_func ? 5:1], resultarray,  ed_func, ivarray);
  return integersToString(resultarray);
}


// des_crypt - This takes the key, the message, whether to encrypt or decrypt and the initialization vector
function des_crypt(keyleft, keyright, messagearray, encrypt, ivarray) {
  // Create the 16 subkeys we will need
  var keys = des_createKeys(keyleft, keyright);
  var m, i, temp;
  // Store the result here
  resultarray = new Array();
  // Loop through each 64 bit chunk of the message
  for (m = 0; m < messagearray.length; m += 2) {
    // <><><><><><><><><><> For Cipher Block Chaining mode, xor the message with the previous result
    if (encrypt) {
      messagearray[m]     ^= (m > 0 ? resultarray[m - 2] : ivarray[0]);
      messagearray[m + 1] ^= (m > 0 ? resultarray[m - 1] : ivarray[1]);
    }
    // First each 64 bit chunk of the message must be permuted according to IP
    // This uses two global variables LEFT and RIGHT to hold the two returns values
    permute(messagearray[m], messagearray[m + 1],  4, 0x0f0f0f0f);
    permute(RIGHT,           LEFT,                16, 0x0000ffff);
    permute(LEFT,            RIGHT,                2, 0x33333333);
    permute(RIGHT,           LEFT,                 8, 0x00ff00ff);
    permute(LEFT,            RIGHT,                1, 0x55555555);
    // LEFT and RIGHT are reversed at this point
    temp  = RIGHT;
    RIGHT = LEFT;
    LEFT  = temp;
    // Now go through and perform the encryption or decryption
    if (encrypt) {
      for (i = 0; i < 32; i += 2) {
        des_cipher(LEFT, RIGHT, keys[i], keys[i + 1]);
      }
    } else {
      for (i = 30; i >= 0; i -= 2) {
        des_cipher(LEFT, RIGHT, keys[i], keys[i + 1]);
      }
    }
    // Now perform IP-1, which is IP in the opposite direction
    // Note that LEFT and RIGHT need to be reversed
    permute(RIGHT, LEFT,   1, 0x55555555);
    permute(LEFT,  RIGHT,  8, 0x00ff00ff);
    permute(RIGHT, LEFT,   2, 0x33333333);
    permute(LEFT,  RIGHT, 16, 0x0000ffff);
    permute(RIGHT, LEFT,   4, 0x0f0f0f0f);
    // LEFT and RIGHT are reversed again
    resultarray[m]     = RIGHT;
    resultarray[m + 1] = LEFT;
    // <><><><><><><><><><> For Cipher Block Chaining mode, xor the message with the previous result
    if (!encrypt) {
      resultarray[m]     ^= (m > 0 ? messagearray[m - 2] : ivarray[0]);
      resultarray[m + 1] ^= (m > 0 ? messagearray[m - 1] : ivarray[1]);
    }
  } // For every 8 characters, or 64 bits in the message
  // Return the result as an array
  return resultarray;
} // End of des_crypt


// Des_cipher - This takes the left and right 32 bits, and the 48 bit subkey
// It sets LEFT = right, and RIGHT = left ^ f (RIGHT, key)
function des_cipher(left, right, leftkey, rightkey) {
  // Tight is expanded into a 48 bit thing according to function E, and XORed with the key
  // This isn't exactly E, because it is tied into how the keys were created
  // Right1 becomes bits: 0 0 4 5 6 7 8 9   0 0 12 13 14 15 16 17..., corresponding to S2, S4, S6 and S8
  // Right2 becomes bits: 0 0 32 1 2 3 4 5  0 0 8 9 10 11 12 13...,   corresponding to S1, S3, S5 and S7
  var right1 = ((right << 1) | (right >>> 31)) ^ leftkey;
  var right2 = ((right >>> 3) | (right << 29)) ^ rightkey;
  // The result is attained by passing these bytes through the S selection functions
  LEFT = right;
  RIGHT = left ^ (spfunction[2][(right1 >>> 24) & 0x3f] | spfunction[4][(right1 >>> 16) & 0x3f]
                | spfunction[6][(right1 >>>  8) & 0x3f] | spfunction[8][right1 & 0x3f]
                | spfunction[1][(right2 >>> 24) & 0x3f] | spfunction[3][(right2 >>> 16) & 0x3f]
                | spfunction[5][(right2 >>>  8) & 0x3f] | spfunction[7][right2 & 0x3f]);
} // End of des_cipher


// des_createKeys - This takes as input a 64 bit key (even though only 56 bits are used)
// as an array of 2 integers, and returns 16 48 bit keys
function des_createKeys(left, right) {
  // Stores the return keys
  var keys = new Array();
  // Now define the left shifts which need to be done
  var shifts = new Array(0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0);
  // Other variables
  var i, lefttemp, righttemp;
  // First the left and right parts must be permuted according to PC-1
  // This uses two global variables LEFT and RIGHT to hold the two returns values
  permute(left, right,   4, 0x0f0f0f0f);
  permute(LEFT, RIGHT, -16, 0x0000ffff);
  permute(LEFT, RIGHT,   2, 0x33333333);
  permute(LEFT, RIGHT, -16, 0x0000ffff);
  permute(LEFT, RIGHT,   1, 0x55555555);
  permute(LEFT, RIGHT,   8, 0x00ff00ff);
  permute(LEFT, RIGHT,   1, 0x55555555);
  // The right side needs to be shifted and to get the last four bits of the left side
  right = (RIGHT << 8) | ((LEFT >>> 20) & 0x000000f0);
  // Left needs to be put upside down
  left  = (LEFT << 24) | ((LEFT << 8)   & 0x00ff0000) | ((LEFT >>> 8) & 0x0000ff00) | ((LEFT >>> 24) & 0x000000f0);
  // Now go through and perform these shifts on the left and right keys
  for (i = 0; i < shifts.length; i++) {
    // Shift the keys either one or two bits to the left
    if (shifts[i]) {
      left  = (left << 2)  | (left >>> 26);
      right = (right << 2) | (right >>> 26);
    } else {
      left  = (left << 1)  | (left >>> 27);
      right = (right << 1) | (right >>> 27);
    }
    left  &= 0xfffffff0;
    right &= 0xfffffff0;

    // Now apply PC-2, in such a way that E is easier when encrypting or decrypting
    // This conversion will look like PC-2 except only the last 6 bits of each byte are used
    // rather than 48 consecutive bits and the order of lines will be according to
    // how the S selection functions will be applied: S2, S4, S6, S8, S1, S3, S5, S7
    lefttemp  = pc2bytes[0][left >>> 28]    | pc2bytes[1][(left          >>> 24) & 0xf]
              | pc2bytes[2][(left >>> 20)   & 0xf] | pc2bytes[3][(left   >>> 16) & 0xf]
              | pc2bytes[4][(left >>> 12)   & 0xf] | pc2bytes[5][(left   >>> 8)  & 0xf]
              | pc2bytes[6][(left >>> 4)    & 0xf];
    righttemp = pc2bytes[7][right >>> 28]   | pc2bytes[8][(right         >>> 24) & 0xf]
              | pc2bytes[9][(right >>> 20)  & 0xf] | pc2bytes[10][(right >>> 16) & 0xf]
              | pc2bytes[11][(right >>> 12) & 0xf] | pc2bytes[12][(right >>> 8)  & 0xf]
              | pc2bytes[13][(right >>> 4)  & 0xf];
    // One more permuation to get the order right
    permute(righttemp, lefttemp, 16, 0x0000ffff);
    keys[i * 2]     = LEFT;
    keys[i * 2 + 1] = RIGHT;
  }
  // Return the keys we've created
  return keys;
} // End of des_createKeys


// stringToIntegers - Converts a string into an array of 32 bit integers
function stringToIntegers(input) {
  var binaries = new Array();
  for (var i = 0; i < input.length; i += 4) {
    binaries[i / 4] = (input.charCodeAt(i)) << 24 | (input.charCodeAt(i + 1) << 16) | (input.charCodeAt(i + 2)) << 8 | input.charCodeAt(i + 3);
  }
  return binaries;
}


// integerToString - Converts an array of 32 bit integers into a string
function integersToString(input) {
  var s = "";
  for (var i = 0; i < input.length; i++) {
    s += String.fromCharCode ((input[i] >>> 24), ((input[i] >>> 16) & 0xff), ((input[i] >>> 8) & 0xff), (input[i] & 0xff));
  }
  return s;
}


// permute - Rotates bits between the left and right integers, leaving the results in global variables
function permute(left, right, shift, mask) {
  var temp = ((left >>> shift) ^ right) & mask;
  LEFT  = right ^ temp;
  RIGHT = left  ^ (temp << shift);
}
