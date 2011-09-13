<div id="feed_and_compare_front">
  <div id="feed_and_compare_front_right" style="float:left;width:49%;border:1px dashed red;">
    ISBN: <?php echo $items[0]['record']['dc:identifier']['dkdcplus:ISBN'][0]; ?><br>
    title: <?php echo $items[0]['title']; ?><br>
    author: <?php echo $items[0]['creators_string']; ?><br>
  </div>
  <div id="feed_and_compare_front_right" style="float:right;width:49%;border:1px dashed red;">
    ISBN: <?php echo $items[1]['record']['dc:identifier']['dkdcplus:ISBN'][0]; ?><br>
    title: <?php echo $items[1]['title']; ?><br>
    author: <?php echo $items[1]['creators_string']; ?><br>
  </div>
</div>
