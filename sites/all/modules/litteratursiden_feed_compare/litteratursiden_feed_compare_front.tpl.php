<?php
/**
 * @file
 */
?>
<div class="feed-and-compare-front clear-block">

  <?php if ($items['status'] == 'empty') { ?>
    <div class="feed_and_compare_empty">
      <h1>Der var desv&aelig;rre ingen materialer som matchede anmeldelser hos Litteratursiden.</h1>
    </div>
  <?php } ?>

  <?php if ($items['status'] == 'error') { ?>
    <div class="feed_and_compare_error">
      <b>error:</b> <?php echo $items['message']; ?>
    </div>
  <?php } ?>

  <?php if (($items['status'] == 'ok') || ($items['status'] == 'notfull')) { ?>
    <?php $i=0; ?>
    <?php foreach ($items['data'] as $key => $item) { ?>
      <?php if (is_numeric($key)) { ?>
        <div class="feed_and_compare_front_item">
          <div class="picture">
            <?php echo theme_image($item['image_url'], '', '', array('width' => '100px'), FALSE); ?><br />
          </div>
          <div class="record">
            <div class="title">
              <?php echo $item['title']; ?><br />
            </div>
            <div class="descr">
              <?php echo $item['abstract']; ?><br />
            </div>
          </div>
        </div>
        <?php $i++; ?>
      <?php } ?>
      <?php
        if ($i == variable_get('litteratursiden_feed_compare_items_on_front', '2')) {
          break;
        }
      ?>
    <?php } ?>
  <?php } ?>

</div>

<div class="bottom-bar">
  <div class="see-more feed_and_compare_front_see_more">
    <?php echo l($conf['see_more_title'], $conf['see_more_link']); ?>
  </div>
</div>
