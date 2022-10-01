package nr3;

import java.util.ArrayList;

public class Notebook {
    private ArrayList<Austattung> parts = null;
    private String name;
    private nType nType;

    public Notebook(String name, nr3.nType nType) {
        parts = new ArrayList<Austattung>();
        this.name = name;
        this.nType = nType;
    }

    public boolean addPart(Austattung part) {
        if (this.getnType().isAllowed(part.getaType())) {
            this.parts.add(part);
            return true;
        } else {
            return false;
        }
    }

    public ArrayList<Austattung> getParts() {
        return parts;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public nr3.nType getnType() {
        return nType;
    }
    public void setnType(nr3.nType nType) {
        this.nType = nType;
    }
}
