#include <iostream>

void pause(std::string msg = "continuar") {
	std::string command = "echo Pressione ENTER para ";
	command.append(msg);
	command.append(" . . . && ( read x 2> nul; rm nul || pause > nul )");

    system(command.c_str());
    return;
}