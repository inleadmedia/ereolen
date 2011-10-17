<?php
// $Id$

/**
 * @file
 * Template to render a Ting collection of books.
 */

foreach ($collection->objects as $obj){
	if($obj->type == 'Netdokument') {
		$netObj = $obj;
//		$netObj = ting_get_object_by_id($obj->id);

  elib_book_cover($netObj);
  $alttext = t('@titel af @forfatter',array('@titel' => $netObj->title, '@forfatter' => $netObj->creators_string));

?>
  <li class="display-book ting-collection ruler-after line clear-block" id="<?php print $netObj->id ?>">

    <div class="picture">
      <?php $image_url = ting_covers_collection_url($netObj->objects[0], '80_x'); ?>
      <?php if ($image_url) { ?>
        <?php print l(theme('image', $image_url, $alttext, $alttext, null, false), $netObj->url, array('html' => true)); ?>
      <?php } ?>
    </div>

    <div class="record">
      <div class="left">
        <h3>
          <?php print l($netObj->title, $netObj->url, array('attributes' => array('class' =>'title'))) ;?> 
        </h3>
        <div class="meta">
          <?php if ($netObj->creators_string) : ?>
            <span class="creator">
              <?php echo t('By !creator_name', array('!creator_name' => l($netObj->creators_string,'ting/search/'.$netObj->creators_string,array('html' => true)))) ?>
            </span>
          <?php endif; ?>
          <div id="<?php print $netObj->objects[0]->localId ?>"></div>
          <?php if ($netObj->date) : ?>
            <span class="publication_date">
              <?php echo t('(%publication_date%)', array('%publication_date%' => $netObj->date)) /* TODO: Improve date handling, localizations etc. */ ?>
            </span>
          <?php endif; ?>
        </div>
        <?php if ($netObj->subjects) : ?>
          <div class="subjects">
            <h4><?php echo t('Subjects:') ?></h4>
            <ul>
              <?php foreach ($netObj->subjects as $subject) : ?>
                <li><?php echo $subject ?></li>
              <?php endforeach; ?>
            </ul>
          </div>
        <?php endif; ?>
      </div>
      <div class="right">
        <?php if ($netObj->abstract) : ?>
          <div class="abstract">
            <p>
              <?php print check_plain($netObj->abstract); ?>
            </p>
          </div>
        <?php endif; ?>
      </div>
    </div>

  </li>

<?php
	}
}
