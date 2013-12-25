<?php
/**
 * @file
 * Template for single reseller.
 */
?>
<div class="item-reseller">
  <hr />
  <p class="name"><?php print $data['seller']->Name; ?></p>
  <?php if (!empty($data['seller']->Logo)) : ?>
  <p class="logo">
    <img height="50" src="<?php print $data['seller']->Logo; ?>" alt="<?php print $data['seller']->Name; ?>" title="<?php print $data['seller']->Name; ?>" />
  </p>
  <?php endif; ?>
  <p class="price"><?php print t('Price: @price DKK', array('@price' => $data['price'])); ?></p>
  <p class="mark-ceritificate"><?php print $data['seller']->Emarkcertificate; ?></p>
  <p class="trust-pilot"><?php print l($data['seller']->Trustpilot, $data['seller']->Trustpilot, array('attributes' => array('target' => '_blank'))); ?></p>
  <p class="buy"><a target="_blank" href="<?php print $data['url']; ?>"><button><?php print t('Buy'); ?></button></a></p>
</div>
