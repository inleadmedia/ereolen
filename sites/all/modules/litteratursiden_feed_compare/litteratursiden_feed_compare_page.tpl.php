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
          <?php print theme('item_list', $readers, t('Reader '), 'span', array('class' => 'contributor')); ?>
          </div>
          <div>
          <?php print theme('item_list', array($item['date']), t('Udgivelses år '), 'span', array('class' => 'year')); ?>
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
          print theme('item_list', $subs, t('Genre '), 'span', array('class' => 'subject'));
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
</div>
