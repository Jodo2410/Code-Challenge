using System;

public static class ContactManagerTests
{
    public static void RunAllTests()
    {
        var book = new ContactManager.ContactBook();

        Console.WriteLine("Starte Tests für ContactBook\n");

        book.AddContact("Anna", "12345");
        book.AddContact("Ben", "67890");

        Test("ContactExists - Anna", book.ContactExists("Anna") == true);
        Test("ContactExists - Clara", book.ContactExists("Clara") == false);

        Test("GetPhoneNumber - Ben", book.GetPhoneNumber("Ben") == "67890");
        Test("GetPhoneNumber - Clara", book.GetPhoneNumber("Clara") == "Kontakt nicht gefunden");
    }

    private static void Test(string testName, bool result)
    {
        if (result)
            Console.WriteLine($"[PASS] {testName}");
        else
            Console.WriteLine($"[FAIL] {testName}");
    }
}