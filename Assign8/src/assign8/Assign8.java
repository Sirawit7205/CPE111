package assign8;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.TreeSet;
import java.util.Iterator;

public class Assign8 {

    TreeSet<TreeNode> dict = new TreeSet();
    
    public static void main(String[] args) {
        int locIndex,locLength;
        String query = new String();
        Scanner input = new Scanner(System.in);
        Assign8 mainMethod = new Assign8();
        TreeNode queryNode = new TreeNode();
        
        mainMethod.readFile("utf8lexitron.csv");            //open dictionary file
 
        mainMethod.printMostMeaning();                      //print most meaning
        
        while(query.compareToIgnoreCase("end")!=0)          //if the query is not "end"
        {
            System.out.println("Enter word: ");
            query = input.nextLine();                       //get query
            query = query.trim().replaceAll("\\s+"," ");    //remove spaces
            queryNode.word = query;                         //copy to tree node
            
            if(mainMethod.dict.contains(queryNode))         //check if this word exists
            {
                TreeSet<TreeNode> found = (TreeSet<TreeNode>) mainMethod.dict.subSet(queryNode,true,queryNode,true);            //get subtree of that word
                
                System.out.printf("Found %s %d times at %d - %d%n",query,found.first().meaning.size(),1,found.first().meaning.size());
                mainMethod.printAllMeanings(found.first());         //print all meanings
            }
            else                                                    //not found
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
                
                TreeNode dataNode = new TreeNode(rawLine);      //new DictNode for every loop + add data to this new node
                
                duplicateCount += insertTree(dataNode);         //insert data to tree and count duplicate
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
        System.out.println("Total remaining tree size: " + dict.size() + " records.");                      //print remaining size
    }
    
    //method for inserting data to tree
    private int insertTree(TreeNode data)
    {
        if(dict.contains(data))                     //if this word already exists...
        {
            TreeSet<TreeNode> found = (TreeSet<TreeNode>) dict.subSet(data, true, data, true);      //...get subtree of that word
            
            if(!found.first().meaning.contains(data.meaning.get(0)))        //if this meaning doesn't exists...
            {
                found.first().meaning.addAll(data.meaning);                 //add this meaning to the tree
                return 0;                                                   //OK
            }
            else            //duplicate
                return 1;   //return 1 to count duplicate
        }
        else                                        //if this word doesn't exists...
        {
            dict.add(data);                         //add this node to the tree
            return 0;                               //OK
        }
    }
    
    //method for printing the word with most meaning
    private void printMostMeaning()
    {
        Iterator<TreeNode> itr = dict.iterator();       //iterator of the tree
        TreeNode maxNode = new TreeNode();
        int maxCount=0;
        
        while(itr.hasNext())                            //check all nodes
        {
            TreeNode cur = itr.next();                  //move iterator to next node
            
            if(cur.meaning.size()>maxCount)             //if this node has more meaning than known max...
            {
                maxCount=cur.meaning.size();            //set new max value
                maxNode=cur;                            //set node of this max value
            }
        }
        
        System.out.println("Maximum meaning word " + maxNode.word + " have " + maxCount + " meaning.");
        printAllMeanings(maxNode);                      //print all meanings of this word
    }
    
    //method for printing all meanings in a tree node
    private void printAllMeanings(TreeNode node)
    {
        for(int i=0;i<node.meaning.size();i++)          //for each meaning in this node...
            System.out.printf("%d) %s %s%n", i+1, node.word, node.meaning.get(i));      //...print meaning
    }
}
