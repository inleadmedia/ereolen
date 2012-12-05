<?php
/**
 * @file
 *
 */
?>
<ul class="publizon-loans-list">
  <?php if (!empty($loans)) { ?>
    <?php foreach ($loans as $loan): ?>
      <li class="display-book biglist"><?php echo theme('publizon_loan', $loan); ?></li>
    <?php endforeach; ?>
  <?php } else { ?>
      <li><?php echo t('You don\'t have any active loans.') ?></li>
  <?php } ?>
</ul>