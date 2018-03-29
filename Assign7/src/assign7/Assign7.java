package assign7;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Collections;

public class Assign7 {
    ArrayList<DictNode> dict = new ArrayList<>();
    
    public static void main(String[] args) {
        int wordCount,remCount,locIndex,locLength;
        String query = new String();
        Scanner input = new Scanner(System.in);
        Assign7 mainMethod = new Assign7();
        DictNode dataNode = new DictNode();
        //DictNode structMethod = new DictNode();
        
        wordCount = mainMethod.readFile("utf8lexitron.csv");
        System.out.println("Total input: " + wordCount + " records.");
        Collections.sort(mainMethod.dict, DictNode.compareByAllFields);
        remCount = mainMethod.removeDuplicate();
        System.out.println("Total duplicate found: " + remCount + " records.");
        System.out.println("Total remaining size: " + mainMethod.dict.size() + " records.");
        mainMethod.printMostMeaning();
        
        while(query.compareToIgnoreCase("end")!=0)
        {
            System.out.println("Enter word: ");
            query = input.nextLine();

            locIndex = Collections.binarySearch(mainMethod.dict, new DictNode(query,"",""), DictNode.compareByWord);
            if(locIndex<0)
                System.out.println("Word "+ query + " not found.");
            else
                mainMethod.printAllMeaning(locIndex);
        }
        
        input.close();
        System.out.println("End program.");
        System.out.println("Program written by 60070501064 Sirawit Lappisatepun");
    }
    
    private int readFile(String filename)
    {
        int wordCount=0;
        String rawLine;
        String [] splitLine = new String[5];
        
        try {
            File data = new File(filename);
            Scanner in = new Scanner(data);
            
            while(in.hasNext())
            {
                DictNode dataNode = new DictNode();
                
                rawLine = in.nextLine();
                if(wordCount==0)
                    rawLine = rawLine.replace("\uFEFF","");         //remove BOM
                splitLine = rawLine.trim().replaceAll("\\s+"," ").split(",");
                dataNode.word = splitLine[0];
                dataNode.meaning = splitLine[1];
                dataNode.type = splitLine[2];
                
                dict.add(dataNode);
                wordCount++;
            }
            
            in.close();
        }
        catch(FileNotFoundException e) {
           System.out.println("File not found."); 
        }
        catch(Exception e) {
            System.out.println("Unknown error.");
        }
        return wordCount;
    }
    
    private int removeDuplicate()
    {
        int removeCount=0,i=0;
        
        while(i<dict.size()-1)
        {
            if(dict.get(i).compareAllFields(dict.get(i+1))==0)
            {
                dict.remove(i+1);
                removeCount++;
            }
            else
                i++;
        }
        return removeCount;
    }
    
    private void printMostMeaning()
    {
        int i=0,maxCount=0, currentCount=1, maxLocation=0;
        
        while(i<dict.size()-1)
        {
            currentCount=1;
            while(dict.get(i).word.compareToIgnoreCase(dict.get(i+currentCount).word)==0)
            {
                currentCount++;
            }
            
            if(currentCount>maxCount)
            {
                maxCount=currentCount;
                maxLocation=i;
                //System.out.println("Max is now " + maxCount + " at " + maxLocation);
            }
            i+=currentCount;
        }
        
        System.out.println("Maximum meaning word " + dict.get(maxLocation).word + " have " + maxCount + " meaning.");
        printDataInRange(maxLocation,maxCount);
    }
    
    private void printAllMeaning(int startIndex)
    {
        int left=startIndex, count=0;
        
        while(left>0&&dict.get(left-1).word.compareToIgnoreCase(dict.get(startIndex).word)==0)
            left--;
        while(left+count<dict.size()&&dict.get(left+count).word.compareToIgnoreCase(dict.get(startIndex).word)==0)
            count++;
        
        System.out.println("Found " + dict.get(startIndex).word + " " + count + " times at " + left + " - " + (left+count-1));
        printDataInRange(left,count);
    }
    
    private void printDataInRange(int startIndex, int length)
    {
        for(int i=startIndex;i<startIndex+length;i++)
            System.out.printf("%d) %s %s(%s)%n", i-startIndex+1, dict.get(i).word, dict.get(i).meaning, dict.get(i).type);
    }
}
