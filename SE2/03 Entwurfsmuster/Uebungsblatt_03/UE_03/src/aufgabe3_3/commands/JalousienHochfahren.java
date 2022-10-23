package aufgabe3_3.commands;

import aufgabe3_3.reciever.Jalousiesteuerung;

public class JalousienHochfahren implements Command {
    private Jalousiesteuerung jalousie;

    public JalousienHochfahren(Jalousiesteuerung jalousie) {
        this.jalousie = jalousie;
    }

    @Override
    public void execute() {
        this.jalousie.hochfahren();
    }
}
