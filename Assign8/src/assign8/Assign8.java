package assign8;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.TreeSet;
import java.util.Collections;
import java.util.Iterator;

public class Assign8 {

    TreeSet<TreeNode> dict = new TreeSet();
    
    public static void main(String[] args) {
        int locIndex,locLength;
        String query = new String();
        Scanner input = new Scanner(System.in);
        Assign8 mainMethod = new Assign8();
        TreeNode queryNode = new TreeNode();
        
        mainMethod.readFile("utf8lexitron.csv");                                    //open dictionary file
 
        mainMethod.printMostMeaning();                                                          //print most meaning
        
        while(query.compareToIgnoreCase("end")!=0)          //if the query is not "end"
        {
            System.out.println("Enter word: ");
            query = input.nextLine();               //get query
            query = query.trim().replaceAll("\\s+"," ");
            queryNode.word = query;
            
            if(mainMethod.dict.contains(queryNode))
            {
                TreeSet<TreeNode> found = (TreeSet<TreeNode>) mainMethod.dict.subSet(queryNode,true,queryNode,true);
                
                System.out.printf("Found %s %d times at %d - %d%n",query,found.first().meaning.size(),1,found.first().meaning.size());
                mainMethod.printAllMeanings(found.first());
            }
            else
                System.out.printf("Word %s not found.%n",query);
        }
        
        input.close();                          //close scanner
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
                    rawLine = rawLine.replace("\uFEFF","");         //remove BOM
                
                TreeNode dataNode = new TreeNode(rawLine);     //new DictNode for every loop + add data to this new node
                
                duplicateCount += insertTree(dataNode);
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
        
        System.out.println("Total input: " + wordCount + " records.");                          //print word count
        System.out.println("Total duplicate found: " + duplicateCount + " records.");                 //print duplicate count
        System.out.println("Total remaining tree size: " + (wordCount - duplicateCount) + " records.");    //print remaining size
        System.out.println("Total remaining tree size: " + dict.size() + " records.");    //print remaining size
    }
    
    private int insertTree(TreeNode data)
    {
        if(dict.contains(data))
        {
            TreeSet<TreeNode> found = (TreeSet<TreeNode>) dict.subSet(data, true, data, true);
            
            if(!found.first().meaning.contains(data.meaning.get(0)))
            {
                found.first().meaning.addAll(data.meaning);
                return 0;
            }
            else
                return 1;
        }
        else
        {
            dict.add(data);
            return 0;
        }
    }
    
    private void printMostMeaning()
    {
        Iterator<TreeNode> itr = dict.iterator();
        TreeNode maxNode = new TreeNode();
        int maxCount=0;
        
        while(itr.hasNext())
        {
            TreeNode cur = itr.next();
            
            if(cur.meaning.size()>maxCount)
            {
                maxCount=cur.meaning.size();
                maxNode=cur;
            }
        }
        
        System.out.println("Maximum meaning word " + maxNode.word + " have " + maxCount + " meaning.");
        printAllMeanings(maxNode);
    }
    
    private void printAllMeanings(TreeNode node)
    {
        for(int i=0;i<node.meaning.size();i++)
            System.out.printf("%d) %s %s%n", i+1, node.word, node.meaning.get(i));
    }
}
