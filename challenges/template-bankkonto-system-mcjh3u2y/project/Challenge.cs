using System;
using System.Collections.Generic;

namespace ChallengeTemplate
{
    /// <summary>
    /// Challenge 17: Bankkonto-System
    /// Implementiere ein einfaches Bankkonto
    /// </summary>
    
    /// <summary>
    /// Bankkonto-Klasse mit grundlegenden Funktionen
    /// </summary>
    public class BankAccount
    {
        
        // private string _accountNumber;
        // private decimal _balance;
        
        /// <summary>
        /// Konstruktor für ein neues Bankkonto
        /// </summary>
        /// <param name="accountNumber">Eindeutige Kontonummer</param>
        /// <param name="initialBalance">Anfangssaldo (optional, Standard: 0)</param>
        public BankAccount(string accountNumber, decimal initialBalance = 0)
        {
            
            // Validiere Parameter und initialisiere Felder
            // Implementierung hier
        }
        
        /// <summary>
        /// Kontonummer abrufen (nur lesbar)
        /// </summary>
        /// <returns>Die Kontonummer</returns>
        public string GetAccountNumber()
        { 
            
            // Implementierung hier
        }
        
        /// <summary>
        /// Aktuellen Kontostand abrufen
        /// </summary>
        /// <returns>Aktueller Saldo</returns>
        public decimal GetBalance()
        {
            
            // Gib aktuellen Saldo zurück
            // Implementierung hier
        }
        
        /// <summary>
        /// Geld einzahlen
        /// </summary>
        /// <param name="amount">Einzuzahlender Betrag</param>
        public void Deposit(decimal amount)
        {
            
            // Validiere Betrag (muss positiv sein)
            // Erhöhe Saldo
            // Implementierung hier
        }
        
        /// <summary>
        /// Geld abheben
        /// </summary>
        /// <param name="amount">Abzuhebender Betrag</param>
        /// <returns>true wenn erfolgreich, false wenn unzureichender Saldo</returns>
        public bool Withdraw(decimal amount)
        {
            
            // Validiere Betrag (muss positiv sein)
            // Prüfe ausreichenden Saldo
            // Reduziere Saldo wenn möglich
            // Implementierung hier
        }
        
        /// <summary>
        /// String-Repräsentation des Kontos
        /// </summary>
        public override string ToString()
        {
            
            // Format: "Konto 12345: 1.500,00 €"
            // Implementierung hier
        }
    }
    
    /// <summary>
    /// Hauptklasse für Template-Kompatibilität
    /// </summary>
    public class Challenge
    {
        /// <summary>
        /// Template-kompatible Solve-Methode
        /// Demonstriert Bankkonto-Funktionalität
        /// </summary>
        public static int Solve(int[] input)
        {
            if (input == null || input.Length < 3)
                return 0;
            
            try
            {
                // Erstelle Testkonto
                var account = new BankAccount("TEST123", input[0]);
                
                // Führe Operationen basierend auf Input durch
                account.Deposit(input[1]);
                bool withdrawSuccess = account.Withdraw(input[2]);
                
                return withdrawSuccess ? (int)account.GetBalance() : -1;
            }
            catch
            {
                return -1;
            }
        }
    }
}