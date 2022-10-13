package aufgabe3.generator;

import aufgabe3.objects.*;

public abstract class AbstractWorldGenerator {
    public abstract Boden bodenLegen();
    public abstract Pflanze pflanzeSetzen();
    public abstract Umrandung umranden();
}
