package assign9;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.Iterator;

public class Assign9 {

    HashMap<String, HashNode> dict = new HashMap<>();
    
    public static void main(String[] args) {
        String query = new String();
        Scanner input = new Scanner(System.in);
        Assign9 mainMethod = new Assign9();
        HashNode queryNode = new HashNode();
        
        mainMethod.readFile("utf8lexitron.csv");            //open dictionary file
 
        mainMethod.printMostMeaning();                      //print most meaning
        
        while(query.compareToIgnoreCase("end")!=0)          //if the query is not "end"
        {
            System.out.println("Enter word: ");
            query = input.nextLine();                       //get query
            query = query.trim().replaceAll("\\s+"," ").toLowerCase();    //remove spaces + convert to lowercase
            queryNode.word = query;                         //copy to tree node
            
            if(mainMethod.dict.containsKey(queryNode.word))                 //check if this word exists
            {
                HashNode found = mainMethod.dict.get(queryNode.word);       //get node
                
                System.out.printf("Found %s %d times at %d - %d%n",query,found.meaning.size(),1,found.meaning.size());
                mainMethod.printAllMeanings(found);         //print all meanings
            }
            else                                            //not found
                System.out.printf("Word %s not found.%n",query);
        }
        
        input.close();                                      //close scanner
        System.out.println("End program.");
        System.out.println("Program written by 60070501064 Sirawit Lappisatepun");   
    }
    
    //method for reading file
    private void readFile(String filename)
    {
        int wordCount=0, duplicateCount=0;
        String rawLine;
        
        try {
            File data = new File(filename);
            Scanner in = new Scanner(data);
            
            while(in.hasNext())                         //if there's still something to read
            {                
                rawLine = in.nextLine();                //get new line
                if(wordCount==0)
                    rawLine = rawLine.replace("\uFEFF","");     //remove BOM
                
                HashNode dataNode = new HashNode(rawLine);      //new DictNode for every loop + add data to this new node
                
                duplicateCount += insertHashMap(dataNode);         //insert data to tree and count duplicate
                wordCount++;                                    //increase count
            }
            
            in.close();                     //close scanner
        }
        catch(FileNotFoundException e) {
           System.out.println("File not found.");       //catch file not found
        }
        catch(Exception e) {
            System.out.println("Unknown error.");       //catch other errors
        }
        
        System.out.println("Total input: " + wordCount + " records.");                                      //print word count
        System.out.println("Total duplicate found: " + duplicateCount + " records.");                       //print duplicate count
        System.out.println("Total remaining size: " + (wordCount - duplicateCount) + " records.");     //print remaining size
        System.out.println("Total remaining hash size: " + dict.size() + " records.");                      //print remaining size
    }
    
    //method for inserting node to HashMap
    private int insertHashMap(HashNode data)
    {
        if(dict.containsKey(data.word))                         //if this word already exists...
        {
            HashNode found = dict.get(data.word);               //get node of this word
            
            if(!found.meaning.contains(data.meaning.get(0)))    //if this meaning doesn't exist...
            {
                found.meaning.addAll(data.meaning);             //add this meaning
                return 0;                                       //OK
            }
            else
                return 1;                                       //duplicate
        }
        else                                                    //else
        {
            dict.put(data.word, data);                          //add this node to HashMap
            return 0;                                           //OK
        }
    }
    
    //method for printing the word with most meaning
    private void printMostMeaning()
    {
        Set set = dict.entrySet();                              //get collections from HashMap
        Iterator itr = set.iterator();                          //get iterator
        HashNode maxNode = new HashNode();                      //node with most meaning
        int maxCount=0;                                         //max count
        
        while(itr.hasNext())                                    //while next node exists...
        {
            Map.Entry cur = (Map.Entry) itr.next();             //get next iterator as Map
            HashNode curNode = (HashNode) cur.getValue();       //get node from Map
            
            if(curNode.meaning.size()>maxCount)                 //if this word has more meaning than max...
            {
                maxCount = curNode.meaning.size();              //set max count
                maxNode = curNode;                              //set max node
            }
        }
        
        System.out.println("Maximum meaning word " + maxNode.word + " have " + maxCount + " meaning.");
        printAllMeanings(maxNode);                      //print all meanings of this word
    }
    
    //method for printing all meanings in a node
    private void printAllMeanings(HashNode data)
    {
        for(int i=0;i<data.meaning.size();i++)                  //for each meaning in this node...
            System.out.printf("%d) %s %s%n", i+1, data.word, data.meaning.get(i));      //...print meaning
    }
}
