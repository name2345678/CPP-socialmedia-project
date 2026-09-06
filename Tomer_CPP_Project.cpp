#include <iostream>
#include <chrono>
#include "Facebook.h"
#include "FacebookInterface.h"
#include "Group.h"
#include "Post.h"
#include "User.h"
#include "TextPost.h"

using namespace std::chrono;

int main()
{
    Facebook facebook;
    
    /*
    User* jeff = facebook.createUser("Jeff Goldblum", sys_days{ year{1980} / May / 15 });  // user[0]
    User* tomer = facebook.createUser("Tomer Ainemer", sys_days{ year{2002} / October / 13 }); // user[1]
    User* barak = facebook.createUser("Barak Obama", sys_days{ year{1999} / January / 1 }); // user[2]

    Group* yoyo = facebook.createGroup("The YoYo Group"); // group[0]
    Group* jojo = facebook.createGroup("The JoJo Group"); // group[1]
    Group* fans = facebook.createGroup("Justin Bieber fan page"); // group[2]

    jeff->createAndAddPost("This my first post! - jeff");
    jeff->createAndAddPost("hi im jeff");

    tomer->createAndAddPost("hi");
    tomer->createAndAddPost("hello i'm Tomer");

    barak->createAndAddPost("Yes we can");
    barak->createAndAddPost("I had a dream");

    yoyo->createAndAddPost("YoYo games!");
    yoyo->createAndAddPost("GG");
    
    jojo->createAndAddPost("Jojo part 6 coming out");
    jojo->createAndAddPost("get ready!!");

    fans->createAndAddPost("okok");
    fans->createAndAddPost("never mind...");

    // Mission 2 operator += tests
    *tomer += *jeff; // tomer adding friend jeff

    *yoyo += *jeff; // jeff joining group yoyo
    *jojo += *tomer; //tomer joining group jojo
    *jojo += *barak; // barak joining group jojo
    

    // Mission 2 operator tests - if most of all works will print those 8 messeges 
    if (TextPost("Yes we can") == *(barak->getPostByIndex(0))) 
        std::cout << "TEST 1: Operator ==  for Post works!\n";
    if (*(fans->getPostByIndex(0)) != *(barak->getPostByIndex(0)))
        std::cout << "TEST 2: Operator != for Post works!\n";
    if (*tomer > *barak)
       std::cout << "TEST 3: Operator > on users works\n";
    if(*tomer == *jeff)
        std::cout << "TEST 4: Operator == on users works!\n";
    if(*tomer != *barak)
        std::cout << "TEST 5: Operator != on users works!\n";
    if(*jojo > *yoyo)
        std::cout << "TEST 6: Operator > on groups works!\n";
    if (*jojo != *yoyo)
        std::cout << "TEST 7: Operator != on groups works!\n";
    if(*jojo >= *yoyo)
        std::cout << "TEST 8: Operator >= on groups works!\n\n";

    // making sure == works
    if (*tomer == *barak)
        std::cout << "ERROR: Operator == on users doesn't work!\n";
    
    */
    FacebookInterface(facebook).run();
}