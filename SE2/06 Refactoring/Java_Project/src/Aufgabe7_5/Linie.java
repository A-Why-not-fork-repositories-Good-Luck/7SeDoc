package Aufgabe7_5;

/**
 * aus [Grechenig et al. 2010]
 */ 
public class Linie {

	private final Punkt punkt = new Punkt();

	private double endX;
	private double endY;
	
	public Linie(double startX, double startY, 
			double endX, double endY) {
		this.punkt.setStartX(startX);
		this.punkt.setStartY(startY);
		this.endX = endX;
		this.endY = endY;
	}
	
	public double distanz() {
		return Math.sqrt(Math.pow(endX - punkt.getStartX(), 2.0) + Math.pow(endY - punkt.getStartY(), 2.0));
	}
	
	public double flaeche() {
		return (Math.abs(endX - punkt.getStartX()) * Math.abs(endY - punkt.getStartY())) / 2;
	}
}
