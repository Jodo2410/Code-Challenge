/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;

public static class Solution {
    public static void CustomSort(int[] nums) {
        MergeSort(nums, 0, nums.Length - 1);
    }

    private static void MergeSort(int[] arr, int left, int right) {
        if (left < right) {
            int mid = (left + right) / 2;
            MergeSort(arr, left, mid);
            MergeSort(arr, mid + 1, right);
            Merge(arr, left, mid, right);
        }
    }

    private static void Merge(int[] arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        int[] L = new int[n1];
        int[] R = new int[n2];

        for (int i = 0; i < n1; i++)
            L[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = arr[mid + 1 + j];

        int iIndex = 0, jIndex = 0, k = left;
        while (iIndex < n1 && jIndex < n2) {
            if (L[iIndex] <= R[jIndex]) {
                arr[k++] = L[iIndex++];
            } else {
                arr[k++] = R[jIndex++];
            }
        }

        while (iIndex < n1) arr[k++] = L[iIndex++];
        while (jIndex < n2) arr[k++] = R[jIndex++];
    }
}