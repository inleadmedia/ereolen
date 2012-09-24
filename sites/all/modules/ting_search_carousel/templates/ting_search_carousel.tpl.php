<?php
/**
 * @file
 *
 *
 * * Available variables:
 * - $searches: Array with each tab search.
 *
 */
?>

<div class="rs-carousel-wrapper rs-carousel-compact">
  <div class="rs-carousel">
    <div class="rs-carousel-inner">
      <div class="ajax-loader"></div>
      <ul class="rs-carousel-runner">
      </ul>
    </div>

    <!-- Only print tabs if there is more than 1 -->
    <?php if (count($searches) > 1): ?>
    <div class="rs-carousel-tabs">
      <ul>
        <?php foreach ($searches as $i => $search): ?>
          <li class="<?php echo ($i == 0) ? 'active' : ''; ?>">
            <a href="#"><?php echo $search['title'] ?></a>
          </li>
        <?php endforeach; ?>
      </ul>
    </div>
    <?php endif; ?>
  </div>
</div>