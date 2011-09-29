<?php
/**
 * @file
 */
?>
<div class="feed-and-compare-page clear-block">
  <ul>
  <?php
  foreach ($items['data'] as $item) {
  ?>
    <li class="clear-block ruler-after">
      <div class="picture unit">
        <?php if ($item['image_url']) { ?>
          <div class="inner left">
            <?php echo theme_image($item['image_url'], '', '', array('width' => '100px'), FALSE); ?><br />
          </div>
        <?php } ?>
      </div>
      <div class="meta unit">
        <div class="inner">
        <h3 class="title">
          <?php print l($item['title'], $item['url'], array('attributes' => array('class' => 'title'))) ;?>
        </h3>
        <div class="author">
          <?php
          echo t(
            'By !creator_name',
            array('!creator_name' => l($item['creators_string'], 'ting/search/' . $item['creators_string'], array('html' => TRUE)))
          );
          ?>
        </div>
        <?php
        if (!empty($item['contributors'])) {
          foreach ($item['contributors'] as $reader) {
            $readers[] = l($reader, 'ting/search/' . $reader);
          }
        ?>
          <div class="reader">
            <?php print theme('item_list', $readers, t('Reader ')); ?>
          </div>
          <div class="year">
            <?php print theme('item_list', array($item['date']), t('Udgivelses år ')); ?>
          </div>
        <?php
        }
        ?>
        <?php
        if (!empty($item['subjects'])) {
          $subs = array();
          foreach($item['subjects'] as $subject) {
            $subs[] = l($subject,'ting/search/'.$subject);
          }
        ?>
          <div class="subject">
            <?php print theme('item_list', $subs, t('Genre ')); ?>
          </div>
        <?php
        }
        ?>
        </div>
      </div>
      <div class="moreinfo unit lastUnit">
        <div class="inner right">
          <div class="abstract">
            <?php print check_plain($item['abstract']); ?>
          </div>
        </div>
      </div>
    </li>
  <?php
  }
  ?>
  </ul>

  <div class="see-more feed_and_compare_page_see_more">
    <?php if ($conf['see_more_link'] != '' && $conf['see_more_title'] != '') { ?>
      <?php echo l($conf['see_more_title'], $conf['see_more_link']); ?>
    <?php } ?>
  </div>

</div>
