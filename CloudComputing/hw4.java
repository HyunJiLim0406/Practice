//package wordcount;
import java.io.IOException;
import java.util.StringTokenizer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;


public class WordCount {
public static class SMapper extends Mapper<Text, Text, Text, IntWritable>{
   private IntWritable result = new IntWritable();
    
public void map(Text key, Text value, Context context) throws IOException, InterruptedException {
String temp = value.toString();
int res = Integer.parseInt(temp);
result.set(res);
context.write(key, result);
}
}
  public static class TokenizerMapper
       extends Mapper<Object, Text, Text, IntWritable>{

    private final static IntWritable one = new IntWritable(1);
    private Text word = new Text();

    public void map(Object key, Text value, Context context
                    ) throws IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString());
      while (itr.hasMoreTokens()) {
        word.set(itr.nextToken());
        context.write(word, one);
      }
    }
  }
  public static class SumReducer
  extends Reducer<Text,IntWritable,Text,IntWritable> {

  private IntWritable result = new IntWritable();

  public void reduce(Text key, Iterable<IntWritable> values,
                  Context context
                  ) throws IOException, InterruptedException {
  int sum = 0;
  for (IntWritable val : values) {
   sum += val.get();
 }
  result.set(sum);
 context.write(key, result);
 //System.out.println(String.format("%s, %d", key.toString(), sum));

 }
 
}
 
  public static class IntSumReducer
       extends Reducer<Text,IntWritable,Text,IntWritable> {

ArrayList<kv> klist = new ArrayList<kv>();
    private IntWritable result = new IntWritable();
    private Text word = new Text();
   
    public void reduce(Text key, Iterable<IntWritable> values,
                       Context context
                       ) throws IOException, InterruptedException {
      int sum = 0;
      String str = "";
      for (IntWritable val : values) {
        sum += val.get();
      }
      str = key.toString();
      klist.add(new kv(str, sum));

 //System.out.println(String.format("%s, %d", key.toString(), sum));

    }
   
    protected void cleanup(Context context) throws IOException, InterruptedException {
    Collections.sort(klist, new cp());
    for(int i=0;i<klist.size();i++) {
    String tmpk = klist.get(i).getKey();
    int tmpv = klist.get(i).getValue();
    word.set(tmpk);
    result.set(tmpv);
    context.write(word, result);
    }
    }
  }
  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    Job job = Job.getInstance(conf, "word count");
    job.setJarByClass(WordCount.class);
    job.setMapperClass(TokenizerMapper.class);
    job.setCombinerClass(SumReducer.class);
    job.setReducerClass(SumReducer.class);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(IntWritable.class);
    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
    job.waitForCompletion(true);
   
    Configuration conf2 = new Configuration();
    Job job2 = Job.getInstance(conf2, "word count");
    job2.setJarByClass(WordCount.class);
    job2.setInputFormatClass(KeyValueTextInputFormat.class);
    job2.setMapperClass(SMapper.class);
    job2.setCombinerClass(IntSumReducer.class);
    job2.setReducerClass(IntSumReducer.class);
    job2.setOutputKeyClass(Text.class);
    job2.setOutputValueClass(IntWritable.class);
    FileInputFormat.addInputPath(job2, new Path(args[1]));
    FileOutputFormat.setOutputPath(job2, new Path(args[2]));
    job2.waitForCompletion(true);
    //System.exit(job2.waitForCompletion(true) ? 0 : 1);
  }
}

class kv{
private String k;
private int v;

public kv(String k, int v) {
this.k = k;
this.v = v;
}
public String getKey() {
return k;
}
public int getValue() {
return v;
}
}

class cp implements Comparator<kv>{

public int compare(kv o1, kv o2) {
int x = o1.getValue();
int y = o2.getValue();

return y-x;
}

}