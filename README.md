# Text2RPG
A text-based game generator

[![Gitter](https://badges.gitter.im/gitterHQ/gitter.svg)](https://gitter.im/legendddhgf_Text2RPG/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

## Potential Ideas:
* Read in text files describing details of games in special format
* Random Room Path Generation
* Stats
* Fighting
* Multiplayer?: Server Hosting vs Database Storing
  * Issue: how do you verify that the other person is playing the same game; remember that this should be cross platform:
  * Current solution: all games function as servers, must pick a unique ID if public, game clients will be provided to connect to servers

### File Format (in regular language form):
```
(
    r $ROOM_NAME
    (d $ROOM_DESC)+
    (g $GROUP_NAME ($PRIORITY)?)*
    (
        t ($ROOM_NAME | $GROUP_NAME)
        o $OPTION_DESC
    )*
)*
```

## Credits:
* Isaak Cherdak (@legendddhgf) - Lead Developer and Product Owner
* August Valera (@4U6U57)- Developer

### OTHERS
* Nathan Whitehead (@nwhitehead) - Designed class assignment for UCSC's CMPS 12B that inspired this project
* http://stackoverflow.com/questions/410980/include-a-text-file-in-a-c-program-as-a-char - compile game instead of game engine
