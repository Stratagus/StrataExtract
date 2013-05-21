#include "main.hpp"

int main(int numberOfArguments, char **commandArguments)
{
    //Create a instance of the StrataExtract configuration to accept arguments
    StrataConfig *mainConfig = new StrataConfig;
    
    CSimpleOpt commandLineArguments(numberOfArguments, commandArguments, CommandLineOptions);
    
    while(commandLineArguments.Next())
    {
        if(commandLineArguments.LastError() == SO_SUCCESS)
        {
            switch (commandLineArguments.OptionId())
            {
                case CMDOPTION_Help:
                        mainConfig->PrintUsage();
                    break;
                    
                case CMDOPTION_GameMediaSourceFilePath:
                    mainConfig->setGameMediaSourcePath(commandLineArguments.OptionArg());
                    break;

                case CMDOPTION_GameMediaDestinationFilePath:
                    mainConfig->setGameMediaDestinationPath(commandLineArguments.OptionArg());
                    std::cout << mainConfig->GameMediaDestinationPath() << '\n';
                    break;
                
                case CMDOPTION_GameConfigurationFilePath:
                    mainConfig->setGameConfiguration(commandLineArguments.OptionArg());
                    break;

            }
        }
        else
        {
            mainConfig->PrintUsage();
            std::cerr << "Invalid Argument: " << commandLineArguments.OptionText() << '\n';
            return -1;
        }
    }
    
    mainConfig->readConfig();
    
    return 0;
}