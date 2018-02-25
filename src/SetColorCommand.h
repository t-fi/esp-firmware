#ifndef SETCOLORCOMMAND_H
#define SETCOLORCOMMAND_H

class SetColorCommand {
public:
    SetColorCommand(int red, int green, int blue, int warmWhite);
    int red;
    int green;
    int blue;
    int warmWhite;
};

#endif // SETCOLORCOMMAND_H
