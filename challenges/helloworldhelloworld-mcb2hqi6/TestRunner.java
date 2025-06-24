/**
 * Test Runner für HelloworldHelloworld
 */
public class TestRunner {
    public static void main(String[] args) {
        System.out.println("🎯 HelloworldHelloworld");
        System.out.println("Führe Tests aus...\n");
        
        try {
            Tests.runAllTests();
        } catch (Exception ex) {
            System.out.println("❌ Fehler: " + ex.getMessage());
            System.exit(1);
        }
        
        System.out.println("\nTests abgeschlossen.");
    }
}