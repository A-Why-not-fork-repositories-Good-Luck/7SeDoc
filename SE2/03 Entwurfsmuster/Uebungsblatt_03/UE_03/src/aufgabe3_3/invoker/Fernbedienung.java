package aufgabe3_3.invoker;

import aufgabe3_3.commands.Command;

public class Fernbedienung {
    private Command einCommand;
    private Command ausCommand;

    public Fernbedienung(Command einCommand, Command ausCommand) {
        this.einCommand = einCommand;
        this.ausCommand = ausCommand;
    }

    public void ein() {
        this.einCommand.execute();
    }
    public void aus() {
        this.ausCommand.execute();
    }
}
