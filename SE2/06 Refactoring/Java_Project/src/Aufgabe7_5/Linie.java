package Aufgabe7_5;

/**
 * Beispielklasse für SE2 Aufgabe 7.5
 */
public class Linie implements LinieInterface {

	private final Aufgabe7_5.punktStart punktStart = new punktStart();
	private final Aufgabe7_5.punktEnde punktEnde = new punktEnde();

	public Linie(double startX, double startY,
				 double endX, double endY) {
		this.punktStart.setStartX(startX);
		this.punktStart.setStartY(startY);
		this.punktEnde.setEndX(endX);
		this.punktEnde.setEndY(endY);
	}

	public double distanz() {
		return Math.sqrt(Math.pow(punktEnde.getEndX() - punktStart.getStartX(), 2.0) + Math.pow(punktEnde.getEndY() - punktStart.getStartY(), 2.0));
	}

	@Override
	public double flaeche() {
		return (Math.abs(punktEnde.getEndX() - punktStart.getStartX()) * Math.abs(punktEnde.getEndY() - punktStart.getStartY())) / 2;
	}
}