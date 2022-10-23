package aufgabe3_3;

import aufgabe3_3.commands.JalousienHerunterfahren;
import aufgabe3_3.commands.JalousienHochfahren;
import aufgabe3_3.commands.LichtAnschalten;
import aufgabe3_3.commands.LichtAusschalten;
import aufgabe3_3.invoker.Fernbedienung;
import aufgabe3_3.reciever.Beleuchtungssteuerung;
import aufgabe3_3.reciever.Jalousiesteuerung;

public class config {
    public static void main(String[] args) {
        Jalousiesteuerung j1 = new Jalousiesteuerung();
        Beleuchtungssteuerung b1 = new Beleuchtungssteuerung();

        JalousienHerunterfahren jRunter = new JalousienHerunterfahren(j1);
        JalousienHochfahren jHoch = new JalousienHochfahren(j1);

        LichtAnschalten bAn = new LichtAnschalten(b1);
        LichtAusschalten bAus = new LichtAusschalten(b1);

        Fernbedienung fBeleuchtung = new Fernbedienung(bAn, bAus);

        Fernbedienung fJalousie = new Fernbedienung(jHoch, jRunter);

        fBeleuchtung.ein();
        fBeleuchtung.aus();

        fJalousie.ein();
        fJalousie.aus();

    }
}
