# vcr
Tape Library File Archiver

# Goals
- Single binary file backup agent
- Archive the most recent file versions only
- Tape library support
- Encrypt files on tape

# Reasons for building this
- Bacula is not suited to my use case and has a lot of moving parts
- Couldn't find another solution which is directly suited to my use case

# Notes
 - Currently I am targeting the IBM TS3310 with this tool as that is the only tape library
I have access to.
 - Program assumes there is one robot and one drive. I plan to fix this later.
 
# Encryption
In order to facilitate storing files securely at rest they will need to be encrypted.
When VCR stores a file onto tape it will store a header that contains the file info
alongside the file data in the cipher text. This will keep all relevant metadata secure
unless accessed by someone with the proper keys. There will also be a clear text header
before the cipher text to indicate the payload length and whether the payload is
encrypted.
