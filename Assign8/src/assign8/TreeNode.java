package assign8;

import java.util.ArrayList;

public class TreeNode implements Comparable<TreeNode>{
    String word;
    ArrayList<String> meaning = new ArrayList<String>();
    
    public TreeNode() {}
    
    public TreeNode(String input)
    {
        String [] buff = input.trim().replaceAll("\\s+"," ").split(",");
        buff[1] = buff[1] + "(" + buff[2] + ")";
        word = buff[0];
        meaning.add(buff[1]);
    }
    
    @Override
    public int compareTo(TreeNode o)
    {
        return (int) word.compareToIgnoreCase(o.word);
    }
}
