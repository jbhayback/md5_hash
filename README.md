# Description
A simple program in C++ that would hash the card numbers using MD5 hash.

# Input
Data that contains card number stored in a text file.
When instantiating the class, pass the file path of the text file as an argument.

# Output
Generate a file of the hashed card number.
When ouputting the md5 hashed card number, call the public meber function outputMD5HashedCardNum().

# Testing
To test the ouput, compile and run the Test.cpp
    1. g++ -o output Test.cpp md5_hash.cpp
    2. Run output.exe
    3. Check within the directory the newly created file having a name of already hashed card number.
    4. Alternatively, you can also open the file using a text editor to see the md5 hashed card number.
