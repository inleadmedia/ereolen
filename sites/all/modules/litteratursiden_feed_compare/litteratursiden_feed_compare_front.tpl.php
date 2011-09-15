<div class="feed_and_compare_front">
  <?php $i = 0; ?>
  <?php foreach ($items as $item) { ?>
    <div class="feed_and_compare_front_item" style="border:1px dashed red;width:48%;float:left;margin:0 10px 10px 0;">
      <div class="feed_and_compare_front_item_image">
          <b>image:</b> <?php echo $item['image_url']; ?><br>
      </div>
      <div class="feed_and_compare_front_item_descr">
        <div class="feed_and_compare_item_isbn">
          <b>ISBN:</b> <?php echo $item['record']['dc:identifier']['dkdcplus:ISBN'][0]; ?><br>
        </div>
        <div class="feed_and_compare_front_item_title">
          <b>title:</b> <?php echo $item['title']; ?><br>
        </div>
        <div class="feed_and_compare_front_item_author">
          <b>author:</b> <?php echo $item['creators_string']; ?><br>
        </div>
        <div class="feed_and_compare_front_item_teaser">
          <b>teaser:</b> <?php echo $item['abstract']; ?><br>
        </div>
      </div>
    </div>
    <?php if ($i++ == 2) break; ?>
  <?php } ?>
</div>
