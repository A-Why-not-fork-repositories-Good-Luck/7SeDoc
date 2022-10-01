package nr3;

public class Austattung {
    private String name;
    private aType aType;

    public Austattung(String name, nr3.aType aType) {
        this.name = name;
        this.aType = aType;
    }

    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public nr3.aType getaType() {
        return aType;
    }
    public void setaType(nr3.aType aType) {
        this.aType = aType;
    }
}
