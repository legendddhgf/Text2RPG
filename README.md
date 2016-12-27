# Text2RPG
A text-based game generator

## Potential Ideas:
* Read in text files describing details of games in special format
* Random Room Path Generation
* Stats
* Fighting
* Multiplayer?: Server Hosting vs Database Storing
*     Issue: how do you verify that the other person is playing the same game; remember that this should be cross platform:
*     Current solution: all games function as servers, must pick a unique ID if public, game clients will be provided to connect to servers

### File Format (in regular language form):
* (
*     r ROOM_NAME
*     (d ROOM_DESC)+
*     (g GROUP_NAME ?PRIORITY)*
*     (
*         t (ROOM_NAME | GROUP NAME)
*         o OPTION_DESC
*     )*
* )*

## Credits:
* Isaak Cherdak - Lead Developer and Product Owner
* August Valera - Developer
* OTHERS
* Nathan Whitehead - Designed class assignment for UCSC's CMPS 12B that inspired this project
* http://stackoverflow.com/questions/410980/include-a-text-file-in-a-c-program-as-a-char - compile game instead of game engine
