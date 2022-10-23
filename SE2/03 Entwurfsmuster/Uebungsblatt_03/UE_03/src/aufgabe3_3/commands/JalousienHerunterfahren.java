package aufgabe3_3.commands;

import aufgabe3_3.reciever.Jalousiesteuerung;

public class JalousienHerunterfahren implements Command {
    private Jalousiesteuerung jalousie;

    public JalousienHerunterfahren(Jalousiesteuerung jalousie) {
        this.jalousie = jalousie;
    }

    @Override
    public void execute() {
        this.jalousie.herunterfahren();
    }
}
