package nr3;

import java.util.ArrayList;

public class nType {
    private String name;
    private ArrayList<aType> allowed = null;

    public nType(String name) {
        allowed = new ArrayList<aType>();
        this.name = name;
    }

    public boolean isAllowed(aType aType) {
        return this.allowed.contains(aType);
    }

    public void addAllowed(aType allowed) {
        this.allowed.add(allowed);
    }

    public ArrayList<aType> getAllowed() {
        return allowed;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
}
