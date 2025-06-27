/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;
using System.Collections.Generic;

public static class ContactManager
{
    public class Contact
    {
        public string Name { get; set; }
        public string PhoneNumber { get; set; }

        public Contact(string name, string phoneNumber)
        {
            Name = name;
            PhoneNumber = phoneNumber;
        }
    }

    public class ContactBook
    {
        private List<Contact> contacts = new List<Contact>();

        public void AddContact(string name, string phoneNumber)
        {
            contacts.Add(new Contact(name, phoneNumber));
        }

        public bool ContactExists(string name)
        {
            foreach (var contact in contacts)
            {
                if (contact.Name == name)
                    return true;
            }
            return false;
        }

        public string GetPhoneNumber(string name)
        {
            foreach (var contact in contacts)
            {
                if (contact.Name == name)
                    return contact.PhoneNumber;
            }
            return "Kontakt nicht gefunden";
        }
    }
}