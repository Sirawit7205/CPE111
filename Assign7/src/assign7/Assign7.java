/*
Assign7: Main class for assignment 7
*/
package assign7;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Collections;

public class Assign7 {
    ArrayList<DictNode> dict = new ArrayList<>();           //ArrayList for storing data
    
    public static void main(String[] args) {
        int wordCount,remCount,locIndex,locLength;
        String query = new String();
        Scanner input = new Scanner(System.in);
        Assign7 mainMethod = new Assign7();
        DictNode dataNode = new DictNode();
        
        wordCount = mainMethod.readFile("utf8lexitron.csv");                                    //open dictionary file
        System.out.println("Total input: " + wordCount + " records.");                          //print word count
        Collections.sort(mainMethod.dict, DictNode.compareByAllFields);                         //sort
        remCount = mainMethod.removeDuplicate();                                                //remove duplicate
        System.out.println("Total duplicate found: " + remCount + " records.");                 //print duplicate count
        System.out.println("Total remaining size: " + mainMethod.dict.size() + " records.");    //print remaining size
        mainMethod.printMostMeaning();                                                          //print most meaning
        
        while(query.compareToIgnoreCase("end")!=0)          //if the query is not "end"
        {
            System.out.println("Enter word: ");
            query = input.nextLine();               //get query

            locIndex = Collections.binarySearch(mainMethod.dict, new DictNode(query,"",""), DictNode.compareByWord);    //find initial location of query
            if(locIndex<0)      //not found
                System.out.println("Word "+ query + " not found.");
            else
                mainMethod.printAllMeaning(locIndex);       //check for all nearby matches and print all
        }
        
        input.close();                          //close scanner
        System.out.println("End program.");
        System.out.println("Program written by 60070501064 Sirawit Lappisatepun");
    }
    
    //method for reading file
    private int readFile(String filename)
    {
        int wordCount=0;
        String rawLine;
        String [] splitLine = new String[5];    //buffer array
        
        try {
            File data = new File(filename);
            Scanner in = new Scanner(data);
            
            while(in.hasNext())                         //if there's still something to read
            {
                DictNode dataNode = new DictNode();     //new DictNode for every loop
                
                rawLine = in.nextLine();                //get new line
                if(wordCount==0)
                    rawLine = rawLine.replace("\uFEFF","");         //remove BOM
                splitLine = rawLine.trim().replaceAll("\\s+"," ").split(",");       //trim + remove excessive spaces + split with ',' to buffer
                dataNode.word = splitLine[0];           //copy to node
                dataNode.meaning = splitLine[1];
                dataNode.type = splitLine[2];
                
                dict.add(dataNode);         //add node
                wordCount++;                //increase count
            }
            
            in.close();                     //close scanner
        }
        catch(FileNotFoundException e) {
           System.out.println("File not found.");       //catch file not found
        }
        catch(Exception e) {
            System.out.println("Unknown error.");       //catch other errors
        }
        return wordCount;                   //return word count
    }
    
    //method for removing duplicate records
    private int removeDuplicate()
    {
        int removeCount=0,i=0;
        
        while(i<dict.size()-1)          //for all nodes in ArrayList
        {
            if(dict.get(i).compareAllFields(dict.get(i+1))==0)  //if the next entry exactly matches this one
            {
                dict.remove(i+1);       //remove it
                removeCount++;          //count duplicate
            }
            else
                i++;                    //continue if not duplicate
        }
        return removeCount;             //return duplicate count
    }
    
    //method for finding and printing most meaning
    private void printMostMeaning()
    {
        int i=0,maxCount=0, currentCount=1, maxLocation=0;
        
        while(i<dict.size()-1)          //for all nodes in ArrayList
        {
            currentCount=1;             //set current meaning count
            while(dict.get(i).word.compareToIgnoreCase(dict.get(i+currentCount).word)==0) //count amount of other meanings of current word
            {
                currentCount++;         //count another meaning
            }
            
            if(currentCount>maxCount)   //if this word has more meaning than last max
            {
                maxCount=currentCount;  //set max count to current
                maxLocation=i;          //set max index to current
            }
            i+=currentCount;            //forward to next word
        }
        
        System.out.println("Maximum meaning word " + dict.get(maxLocation).word + " have " + maxCount + " meaning.");
        printDataInRange(maxLocation,maxCount);     //print all meanings
    }
    
    //method for finding bounds of a word from binarySearch and prints them
    private void printAllMeaning(int startIndex)
    {
        int left=startIndex, count=0;
        
        while(left>0&&dict.get(left-1).word.compareToIgnoreCase(dict.get(startIndex).word)==0)      //go left if the previous word still matches
            left--;     //go left
        while(left+count<dict.size()&&dict.get(left+count).word.compareToIgnoreCase(dict.get(startIndex).word)==0)      //count from left bound if the word still matches
            count++;    //increase count
        
        System.out.println("Found " + dict.get(startIndex).word + " " + count + " times at " + left + " - " + (left+count-1));
        printDataInRange(left,count);       //print all meanings
    }
    
    //method for printing word meaning, starting from startIndex, amount specified by length
    private void printDataInRange(int startIndex, int length)
    {
        for(int i=startIndex;i<startIndex+length;i++)       //from startIndex for length
            System.out.printf("%d) %s %s(%s)%n", i-startIndex+1, dict.get(i).word, dict.get(i).meaning, dict.get(i).type);
    }
}
